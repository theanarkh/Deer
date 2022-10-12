#include "threadpool.h"

namespace Deer {
    
    ThreadPool::ThreadPool(int count) {
        for (int i = 0; i < count; i++) {
            threads.push_back(std::thread(&ThreadPool::worker, this));
        }
    }

    void ThreadPool::submit(thread_pool_request* request) {
        std::lock_guard<std::mutex> lock(mutex);
        requests.push_back(request);
        condition_variable.notify_one();
    }

    void ThreadPool::stop() {
        for (auto& thread: threads) {
            thread.join();
        }
    }

    void ThreadPool::worker() {
        while (true) {
            std::unique_lock<std::mutex> lock(mutex);
            while ((requests).size() == 0)
            {
                (condition_variable).wait(lock);
            }
            thread_pool_request* request = requests.front();
            requests.pop_front();
            request->work_fn(request);
            // TODO
        }
    }

    ThreadPool ThreadPool::default_thread_pool;

    void ThreadPool::Init(v8::Isolate* isolate, v8::Local<v8::Object> target) {
        Local<v8::ObjectTemplate> threadpool = v8::ObjectTemplate::New(isolate);
        setMethod(isolate, threadpool, "test_thread_pool", [](V8_ARGS) {
            thread_pool_request* request = new thread_pool_request();
            request->work_fn = [](thread_pool_request*) {
                printf("hello, i am in thread!");
            },
            request->done_fn = [](thread_pool_request*) {
                printf("hello, i am in main thread!");
            },
            request->data = nullptr;
            default_thread_pool.submit(request);
        });
        setObjectValue(isolate, target, "threadpool", threadpool->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
    }
}