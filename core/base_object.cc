#include "base_object.h"

Deer::BaseObject::BaseObject(Environment* env, Local<Object> object)
    : _env(env), _object(env->GetIsolate(), object) {
    object->SetAlignedPointerInInternalField(0, static_cast<void*>(this));
}

Deer::BaseObject::~BaseObject() {
    object()->SetAlignedPointerInInternalField(0, nullptr);
    _object.Reset();
}

Environment* Deer::BaseObject::env() const {
  return _env;
}

Local<Object> Deer::BaseObject::object() {
  return PersistentToLocal::Strong(_object);
}

void Deer::BaseObject::ClearWeak() {
  _object.ClearWeak();
}

void Deer::BaseObject::MakeWeak() {
  _object.SetWeak(
      this,
      [](const WeakCallbackInfo<BaseObject>& data) {
        BaseObject* obj = data.GetParameter();
        obj->_object.Reset();
        delete obj;
      },
      WeakCallbackType::kParameter);
}