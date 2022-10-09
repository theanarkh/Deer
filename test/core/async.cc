#include "async.h"

namespace Deer {

    Async::Async(Environment* env, Local<Object> object): Deer::BaseObject(env, object) {}

    void Async::makeCallback(char* name, int argc, v8::Local<v8::Value>* argv) {
        Local<Object> obj = object();
        Local<Value> cb;
        Local<Context> context = env()->GetContext();
        Local<String> key = newStringToLcal(env()->GetIsolate(), name);       
        obj->Get(context, key).ToLocal(&cb);
        if (cb->IsFunction()) {
            cb.As<v8::Function>()->Call(context, obj, argc, argv);
        }
    };
}
