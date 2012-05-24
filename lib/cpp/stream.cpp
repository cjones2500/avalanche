#include <sstream>
#include <zmq.hpp>
#include <assert.h>
#include <curl/curl.h>  
#include <TBuffer.h>
#include <TBufferFile.h>
#include <TClass.h>
#include <TStreamerInfo.h>
#include <TList.h>

#include "json/json.h"
#include "json/reader.h"
#include "json/value.h"

#include "avalanche.hpp"
#include "stream.hpp"

#define debug_print(fmt, ...)   do {if(DEBUG) fprintf(stderr, fmt, __VA_ARGS__);} while(0)

namespace avalanche {

    void* watchDispatcher(void* arg) {
        dispatcherState* s = (dispatcherState*) arg;

        zmq::socket_t* socket = new zmq::socket_t(*s->context, ZMQ_SUB);
        socket->setsockopt(ZMQ_SUBSCRIBE, "", 0);
        zmq::socket_t* si_socket = new zmq::socket_t(*s->context, ZMQ_REQ);
        int linger = 0;
        std::map<std::string, TList*> si_list;

        try {
            socket->connect(s->address.c_str());
            si_socket->connect(s->si_address.c_str());
            si_socket->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
        }
        catch(zmq::error_t &e) {
            std::cout << "dispatcher " << s->address << std::endl;
            std::cout << "client connection failed with errno: " << e.num();
            std::cout << ", " << e.what() << std::endl;
            assert(0);
        }
        std::cout << "dispatcher " << s->address << " connected." << std::endl;

        zmq::pollitem_t items [] = {
            {*socket, 0, ZMQ_POLLIN, 0}
        };

        while (!*s->is_cancelled) {
            try {
                zmq::poll(items, 1, 1000);
            }
            catch(zmq::error_t &e) {
                std::cout << "dispatcher " << s->address << std::endl;
                std::cout << "SUB socket poll failed with errornum: " << e.num();
                std::cout << ", " << e.what() << std::endl;
                sleep(1);
                continue;
            }
            if (items[0].revents & ZMQ_POLLIN) {
                zmq::message_t* msg = new zmq::message_t();
                bool rc;
                try {
                    rc = socket->recv(msg);
                }
                catch (zmq::error_t &e) {
                    std::cout << "dispatcher " << s->address << std::endl;
                    std::cout << "SUB socket recv failed with errornum: " << e.num();
                    std::cout << ", " << e.what() << std::endl;
                    rc = false;
                }
                if (!rc) { //buffer empty
                    delete msg;
                    continue;
                }
                if (msg->size() < 2) {
                    debug_print("dispatcher %s, empty message skipped\n", s->address.c_str());
                    delete msg;
                    continue;
                }

                TBufferFile* bf = new TBufferFile(TBuffer::kRead, msg->size(), msg->data(), false);
                bf->InitMap();
                TClass* cl = bf->ReadClass();
                bf->Reset();

                if (!si_list.count(cl->GetName())) {
                    zmq::message_t* msg_si_req = new zmq::message_t(msg->size());
                    memcpy((void*)msg_si_req->data(), msg->data(), msg->size());
                    try {
                        si_socket->send(*msg_si_req);
                    }
                    catch(zmq::error_t &e) {
                        std::cout << "dispatcher " << s->address << std::endl;
                        std::cout << "streamer request failed with errno: " << e.num();
                        std::cout << ", " << e.what() << std::endl;
                        assert(0);
                    }
     
                    zmq::pollitem_t si_items [] = {
                        {*si_socket, 0, ZMQ_POLLIN, 0}
                    };
                    try {
                        zmq::poll(si_items, 1, 1000000); //this blocks s->is_cancelled on purpose
                    }
                    catch(zmq::error_t &e) {
                        std::cout << "dispatcher " << s->address << std::endl;
                        std::cout << "REQ socket poll errornum: " << e.num();
                        std::cout << " " << e.what() << std::endl;
                        sleep(1);
                        continue;
                    }
                    if (si_items[0].revents & ZMQ_POLLIN) {
                        zmq::message_t* msg_si_rep = new zmq::message_t();
                        try {
                            rc = si_socket->recv(msg_si_rep);
                        }
                        catch (zmq::error_t &e) {
                            std::cout << "dispatcher " << s->address << std::endl;
                            std::cout << "REQ socket recv failed with errornum: " << e.num();
                            std::cout << ", " << e.what() << std::endl;
                            rc = false;
                        }
                        if (!rc) { //buffer empty
                            delete msg_si_rep;
                            continue;
                        }

                        TBufferFile* si_bf = new TBufferFile(TBuffer::kRead, msg_si_rep->size(), msg_si_rep->data(), false);
                        TList* list = 0;
                        if (msg_si_rep->size() < 2) { //empty class is one byte
                            debug_print("dispatcher %s, empty streamer response for class: %s\n",
                                s->address.c_str(), cl->GetName());
                        }
                        else {
                            si_bf->InitMap();
                            TClass* si_cl = si_bf->ReadClass();
                            si_bf->Reset();
                            if (strcmp(si_cl->GetName(),TList::Class()->GetName())) {
                                debug_print("dispatcher %s, streamer info for class: %s wasn't TList but %s\n",
                                    s->address.c_str(), cl->GetName(), si_cl->GetName());
                            }
                            else {
                                list = (TList*)si_bf->ReadObjectAny(TList::Class());
                            }
                        }
                        if (list) {
                            //stolen from TSocket::RecvStreamerInfos, regular classes first, followed by stl
                            TIter next(list);
                            TStreamerInfo *info;
                            TObjLink *lnk = list->FirstLink();
                            while (lnk) {
                                info = (TStreamerInfo*)lnk->GetObject();
                                TObject *element = info->GetElements()->UncheckedAt(0);
                                Bool_t isstl = element && strcmp("This",element->GetName())==0;
                                if (!isstl) {
                                    debug_print("class %s streamers before: %u ", info->GetName(),
                                        TClass::GetClass(info->GetName())->GetStreamerInfos()->GetEntries());
                                    info->BuildCheck();
                                    debug_print("after: %u\n", TClass::GetClass(info->GetName())->GetStreamerInfos()->GetEntries());
                                }
                                lnk = lnk->Next();
                            }
                            lnk = list->FirstLink();
                            while (lnk) {
                                info = (TStreamerInfo*)lnk->GetObject();
                                TObject *element = info->GetElements()->UncheckedAt(0);
                                Bool_t isstl = element && strcmp("This",element->GetName())==0;
                                if (isstl) {
                                    debug_print("class %s streamers before: %u ", info->GetName(),
                                        TClass::GetClass(info->GetName())->GetStreamerInfos()->GetEntries());
                                    info->BuildCheck();
                                    debug_print("after: %u\n", TClass::GetClass(info->GetName())->GetStreamerInfos()->GetEntries());
                                }
                                lnk = lnk->Next();
                            }
                            //end stolen
                        }
                        si_list[cl->GetName()] = (TList*) list;

                        delete si_bf;
                        delete msg_si_rep;
                    }
                    else { //bad, we haven't received the streamer
                        std::cout << "dispatcher " << s->address << std::endl;
                        std::cout << "no streamer received for " << cl->GetName() << std::endl;
                        delete si_socket;
                        try {
                            si_socket = new zmq::socket_t(*s->context, ZMQ_REQ);
                            si_socket->connect(s->si_address.c_str());
                            si_socket->setsockopt(ZMQ_LINGER, &linger, sizeof(linger));
                        }
                        catch(zmq::error_t &e) {
                            std::cout << "dispatcher " << s->address << std::endl;
                            std::cout << "REQ connection failed with errno: " << e.num();
                            std::cout << ", " << e.what() << std::endl;
                            assert(0); //no resp and can not connect, too bad
                        }

                        //give it a try anyways
                        si_list[cl->GetName()] = 0;
                    }
                }

                if (!*s->is_cancelled) {
                    TObject* o = (TObject*) bf->ReadObjectAny(cl);
                    pthread_mutex_lock(s->queueMutex);
                    s->queue->push(o);
                    pthread_mutex_unlock(s->queueMutex);
                }

                delete bf;
                delete msg;
            }
        }

        delete si_socket;
        delete socket;
        std::map<std::string, TList*>::iterator it;
        for (it = si_list.begin(); it != si_list.end(); ++it) {
            delete (*it).second;
        }
        
        return NULL;
    }

    void* watchDB(void* arg) {
        dbState* s = (dbState*) arg;

        // set up curl connection
        CURL* curl;
        curl = curl_easy_init();
        assert(curl);
        std::ostringstream data("");

        // get db info to get current update_seq
        std::string query = s->host + "/" + s->dbname;
        curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ptr_to_stream);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        curl_easy_perform(curl);

        Json::Reader* reader = new Json::Reader();
        Json::Value v;
        assert(reader->parse(data.str(), v, false));
        assert(v.isObject());
        int update_seq = v["update_seq"].asInt();

        std::stringstream update_seq_ss;
        update_seq_ss << update_seq;

        // listen to the changes feed forever
        query = s->host + "/" + s->dbname + "/_changes?feed=continuous&include_docs=true&heartbeat=50000&since=" + update_seq_ss.str();
        if (s->filterName != "") {
            query += ("&filter=" + s->filterName);
        }

        curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, db_change_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, s);

        // start request
        curl_easy_perform(curl);

        curl_easy_cleanup(curl);
        return NULL;
    }

    size_t ptr_to_stream(void* ptr, size_t size, size_t nmemb, void* stream) {
        std::ostringstream* doc = static_cast<std::ostringstream*>(stream);
        char* data = static_cast<char*>(ptr);

        for (size_t i=0; i < size*nmemb; i++)
            (*doc) << data[i];

        return size * nmemb;
    }

    size_t db_change_callback(void* ptr, size_t size, size_t nmemb, void* state) {
        dbState* s = (dbState*) state;

        std::ostringstream doc("");
        ptr_to_stream(ptr, size, nmemb, (void*)&doc);

        if (doc.str().find("{") != std::string::npos) {
            Json::Reader reader;
            Json::Value v;
            reader.parse(doc.str(), v, false);

            if (v.isObject()) {
                TObject* o = (*(s->map))(v["doc"]);
                if (o) {
                    pthread_mutex_lock(s->queueMutex);
                    s->queue->push(o);
                    pthread_mutex_unlock(s->queueMutex);
                }
            }
        }

        return size * nmemb;
    }

} // namespace avalanche

