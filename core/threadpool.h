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

typedef void(*dummy)();

namespace Deer {

    class ThreadPool {
        public:
            ThreadPool(int count = 10);
            void submit(dummy fn);
            void worker();  
            static void Init(Isolate* isolate, Local<Object> target);
            static ThreadPool default_thread_pool;
        private:
            std::vector<std::thread> threads;
            std::condition_variable condition_variable;
            std::deque<dummy> requests;
            std::mutex mutex;  
    };
}

#endif