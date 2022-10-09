#include "console.h"

void Deer::Console::log(V8_ARGS) {
    V8_ISOLATE
    String::Utf8Value str(isolate, args[0]);
    Log(*str);
}

void Deer::Console::Init(Isolate* isolate, Local<Object> target) {
  Local<ObjectTemplate> console = ObjectTemplate::New(isolate);
  setMethod(isolate, console, "log", Deer::Console::log);
  setObjectValue(isolate, target, "console", console->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}

