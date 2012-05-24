#include <zmq.hpp>
#include <TBufferFile.h>
#include <TClass.h>
#include <TList.h>
#include <TVirtualStreamerInfo.h>
#include <TThread.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include <pthread.h>
#include "avalanche.hpp"

namespace avalanche {

struct si_worker_arg {
    zmq::context_t* context;
    std::string* si_address;
    bool* is_cancelled;
};

static void* si_worker(void* arg) {

    /**
     * customized TBufferFile
     *
     * TagStreamerInfo overloaded to store the list of used StreamerInfos
     */

    class SIMessage : TBufferFile {
        public:
            SIMessage(Int_t bufsiz = TBuffer::kInitialSize) :
                TBufferFile(TBuffer::kWrite, bufsiz),
                si_list(new TList()),
                si_buffer(new TBufferFile(TBuffer::kWrite, TBuffer::kInitialSize)) {};
            ~SIMessage() {
                delete si_list;
                delete si_buffer;
            }
            void TagStreamerInfo(TVirtualStreamerInfo *info) {
                si_list->Add(info);
            }
            void* Buffer() {
                return si_buffer->Buffer();
            }
            int Length() {
                return si_buffer->Length();
            }
            void Process(const TObject* o) {
                Reset();
                si_list->Clear();
                si_buffer->Reset();
                int rc = WriteObjectAny(o, TObject::Class());
                if (rc != 1) {
                    return;
                }
                si_buffer->WriteObjectAny(si_list, TList::Class());
            }
        private:
            TList* si_list;
            TBufferFile* si_buffer;
    }
    si_msg;

    struct si_worker_arg* si_arg = (struct si_worker_arg*)arg;
    zmq::socket_t* socket_si = new zmq::socket_t(*si_arg->context, ZMQ_REP);
    socket_si->bind(si_arg->si_address->c_str());

    zmq::pollitem_t items [] = {
        {*socket_si, 0, ZMQ_POLLIN, 0}
    };

    while (!*si_arg->is_cancelled) {
        try {
            zmq::poll(items, 1, 1000);
        }
        catch(zmq::error_t &e) {
            std::cout << "warning: streamer info server " << *si_arg->si_address << std::endl;
            std::cout << "REP socket poll failed with errnum: " << e.num();
            std::cout << ",  " << e.what() << std::endl;
            continue;
        }

        if (items[0].revents & ZMQ_POLLIN) {
            zmq::message_t msg;
            socket_si->recv(&msg);

            TBufferFile* req = new TBufferFile(TBuffer::kRead, msg.size(), msg.data(), false);
            req->InitMap();

            TClass* cl = req->ReadClass();
            if ((void*) cl == (void*) -1 || (void*) cl == (void*) 0) {
                std::cout << "server " << *si_arg->si_address << std::endl;
                std::cout << "error decoding streamer info request" << std::endl;
                msg.rebuild();
                socket_si->send(msg);
                delete req;
                continue;
            }

            req->Reset();
            TObject* o = (TObject*) req->ReadObjectAny(cl);
            si_msg.Process(o);

            int bufferSize = si_msg.Length();
            zmq::message_t message(bufferSize);
            memcpy((void*)message.data(), si_msg.Buffer(), bufferSize);
            socket_si->send(message);
            delete req; 
        }
    }
    delete socket_si;
    return NULL;
}

server::server(std::string _addr) {
    // set up zeromq sockets
    address = _addr;
    context = new zmq::context_t(1);
    socket = new zmq::socket_t(*context, ZMQ_PUB);
    socket->bind(address.c_str());
    bf = new TBufferFile(TBuffer::kWrite);

    if (address.find("tcp://") != std::string::npos ||
        address.find("pgm://") != std::string::npos ||
        address.find("epgm://") != std::string::npos) {

        std::istringstream str_port(address.substr(address.rfind(':') + 1));
        unsigned int num_port = 0;
        str_port >> num_port;
        num_port++;
        std::ostringstream str_si_address;
        str_si_address << address.substr(0, address.rfind(':') + 1);
        str_si_address << num_port;
        si_address = str_si_address.str();
    }
    else if (address.find("ipc://") != std::string::npos ||
        address.find("inproc://") != std::string::npos) {

        si_address = address + "_si";
    }   
    else {
        assert(0);
    }

    TThread::Initialize();
    si_arg = new struct si_worker_arg((struct si_worker_arg) {context, &si_address, new bool(false)});
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&si_handle, &attr, avalanche::si_worker, si_arg);

    // PUB/SUB sockets are slow-joiners
    usleep(1000000);
    zmq::message_t message;
    socket->send(message);

    std::cout << "server " << address << " started." << std::endl;
}

server::~server() {
    delete bf;
    delete socket;
    *si_arg->is_cancelled = true;
    usleep(10000);
    delete context;
    delete si_arg;
    std::cout << "server " << address << " stopped." << std::endl;
}

int server::sendObject(TObject* o) const {
    bf->Reset();

    if (bf->WriteObjectAny(o, TObject::Class()) != 1)
        return 1;

    // zmq_send() frees the memory the message points to, which in our case is
    // owned by a TBufferFile. hence we need to make a copy.
    int bufferSize = bf->Length();
    zmq::message_t message(bufferSize);
    memcpy((void*)message.data(), bf->Buffer(), bufferSize);
    bool rc = socket->send(message);

    if (!rc)
        return 1;
    return 0;
}

} // namespace avalanche

