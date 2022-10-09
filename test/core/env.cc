
#include "env.h"

class Environment;

Deer::Env::Environment::Environment(Local<Context> context) {
    Isolate* isolate = context->GetIsolate();
    context_.Reset(isolate, context);
    isolate_ = isolate;
    context->SetAlignedPointerInEmbedderData(Deer::Env::CONTEXT_INDEX, this);
}

Deer::Env::Environment* Deer::Env::Environment::GetEnvByContext(Local<Context> context) {
    return (Environment*)context->GetAlignedPointerFromEmbedderData(Deer::Env::CONTEXT_INDEX);
}