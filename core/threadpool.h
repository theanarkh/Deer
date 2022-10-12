#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread> 
#include <deque>
#include <vector>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include "util.h"
#include "env.h"

namespace Deer {
    struct thread_pool_request;
    typedef void(*work)(thread_pool_request*);
    typedef void(*done)(thread_pool_request*);

    struct thread_pool_request {
        work work_fn;
        done done_fn;
        void *data;
    };
    class ThreadPool {
        public:
            ThreadPool(int count = 10);
            void submit(thread_pool_request* request);
            void worker();  
            void stop();  
            static void Init(Isolate* isolate, Local<Object> target);
            static ThreadPool default_thread_pool;
        private:
            std::vector<std::thread> threads;
            std::condition_variable condition_variable;
            std::deque<thread_pool_request*> requests;
            std::mutex mutex;
    };
}

#endif