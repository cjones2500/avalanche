#include <zmq.hpp>
#include <pthread.h>
#include <stdint.h>
#include <string>
#include <cassert>
#include <sstream>
#include <iostream>

#include <TObject.h>
#include <TClass.h>
#include <TThread.h>

#include "avalanche.hpp"
#include "stream.hpp"

namespace avalanche {

    client::client() :
        queueMutex(new pthread_mutex_t()),
        context(new zmq::context_t(1))
    {
        pthread_mutex_init(queueMutex, NULL);
        TThread::Initialize();
    }

    client::~client() {
        std::map<pthread_t*, streamState*>::iterator it;
        for (it = threads.begin(); it != threads.end(); ++it) {
            *(*it).second->is_cancelled = true;
        }
        usleep(10000);

        delete context;

        pthread_mutex_lock(queueMutex);
        while (!queue.empty()) {
            queue.pop();
        }
        pthread_mutex_unlock(queueMutex);

        for (it = threads.begin(); it != threads.end(); ++it) {
            delete (*it).first;
            delete (*it).second;
        }
        threads.erase(threads.begin(), threads.end());
        streams.erase(streams.begin(), streams.end());

        pthread_mutex_lock(queueMutex); //unlocking unlocked mutex is undefined and threads might be running
        pthread_mutex_unlock(queueMutex); //destroying locked mutex is undefined
        pthread_mutex_destroy(queueMutex);
    }

    void client::addDispatcher(std::string _addr) {
        pthread_t* dispatcherThread = new pthread_t();
        dispatcherState* s = new dispatcherState();
        s->queue = &queue;
        s->queueMutex = queueMutex;
        s->is_cancelled = new bool(false);
        s->context = context;
        streams["dispatcher"].push_back(_addr);
        threads[dispatcherThread] = dynamic_cast<streamState*>(s);

        std::string si_address;
        if (_addr.find("tcp://") != std::string::npos ||
            _addr.find("pgm://") != std::string::npos ||
            _addr.find("epgm://") != std::string::npos) {

            std::istringstream str_port(_addr.substr(_addr.rfind(':') + 1));
            unsigned int num_port = 0;
            str_port >> num_port;
            num_port++;
            std::ostringstream str_si_address;
            str_si_address << _addr.substr(0, _addr.rfind(':') + 1);
            str_si_address << num_port;
            si_address = str_si_address.str();
        }
        else if (_addr.find("ipc://") != std::string::npos ||
            _addr.find("inproc://") != std::string::npos) {

            si_address = _addr + "_si";
        }   
        else {
            assert(0);
        }

        s->address = _addr;
        s->si_address = si_address;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_create(dispatcherThread, &attr, watchDispatcher, (void*) s);
    }

    void client::addDB(std::string _host, std::string _dbname, docObjectMap _map, std::string _filterName) {
        streams["couchdb"].push_back(_host + "/" + _dbname);

        // launch couchdb-watching thread with given state
        dbState* s = new dbState();
        s->queue = &queue;
        s->queueMutex = queueMutex;
        s->is_cancelled = new bool(false);
        s->host = _host;
        s->dbname = _dbname;
        s->map = _map;
        s->filterName = _filterName;

        pthread_t* couchThread = new pthread_t();
        pthread_create(couchThread, NULL, watchDB, (void*) s);
        threads[couchThread] = s;
    }

    TObject* client::recv(bool blocking) {
        while (blocking && queue.empty())
            continue;

        if (queue.empty()) {
            usleep(1000);
            return NULL;
        }
        else {
            pthread_mutex_lock(queueMutex);
            //slow since it goes through streamer
            //TObject* o = (TObject*) queue.front()->IsA()->New();
            //may crash when virtual method called
            //memcpy(o, queue.front(), o->IsA()->Size());
            //requires copy constructor
            TObject* o = queue.front()->Clone();
            queue.pop();
            pthread_mutex_unlock(queueMutex);
            return o;
        }
    }
} // namespace avalanche

