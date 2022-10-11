#include "threadpool.h"

namespace Deer {
    
    ThreadPool::ThreadPool(int count) {
        for (int i = 0; i < count; i++) {
            threads.push_back(std::thread(&ThreadPool::worker, this));
        }
    }

    void ThreadPool::submit(dummy fn) {
        std::lock_guard<std::mutex> lock(mutex);
        requests.push_back(fn);
        condition_variable.notify_one();
    }

    void ThreadPool::worker() {
        while (true) {
            std::unique_lock<std::mutex> lock(mutex);
            while ((requests).size() == 0)
            {
                (condition_variable).wait(lock);
            }
            dummy fn = requests.front();
            requests.pop_front();
            fn();
        }
    }

    ThreadPool ThreadPool::default_thread_pool;

    void ThreadPool::Init(v8::Isolate* isolate, v8::Local<v8::Object> target) {
        Local<v8::ObjectTemplate> threadpool = v8::ObjectTemplate::New(isolate);
        setMethod(isolate, threadpool, "test_thread_pool", [](V8_ARGS) {
            default_thread_pool.submit([]() {
                printf("hello, i am in thread!");
            });
        });
        setObjectValue(isolate, target, "threadpool", threadpool->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
    }
}