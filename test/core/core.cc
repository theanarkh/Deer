
#include "core.h"

void Deer::Core::register_builtins(Isolate * isolate, Local<Object> No) {
    Local<Object> target = Object::New(isolate);
    Process::Init(isolate, target);
    Loader::Init(isolate, target);
    Util::Init(isolate, target);
    Socket::Init(isolate, target);
    Console::Init(isolate, target);
    setObjectValue(isolate, No, "buildin", target);
}
