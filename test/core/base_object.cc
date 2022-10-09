#include "base_object.h"

Deer::BaseObject::BaseObject(Environment* env, Local<Object> object)
    : _env(env), _object(env->GetIsolate(), object) {
    object->SetAlignedPointerInInternalField(0, static_cast<void*>(this));
}

Deer::BaseObject::~BaseObject() {
    object()->SetAlignedPointerInInternalField(0, nullptr);
}

Environment* Deer::BaseObject::env() const {
  return _env;
}

Local<Object> Deer::BaseObject::object() {
  return PersistentToLocal::Strong(_object);
}
