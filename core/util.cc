
#include "util.h"

void Deer::Util::Log(const char * str) {
    write(STDOUT_FILENO, str, strlen(str));
}

Local<String> Deer::Util::newStringToLcal(Isolate * isolate, const char * str, NewStringType type) {
    return String::NewFromUtf8(isolate, str, type, strlen(str)).ToLocalChecked();
}

MaybeLocal<String> Deer::Util::newString(Isolate * isolate, const char * str, NewStringType type) {
    return String::NewFromUtf8(isolate, str, type, strlen(str));
}

// 设置对象的属性，属性为函数
void Deer::Util::setMethod(Isolate *isolate, 
                Local<ObjectTemplate> 
                recv, const char *name, 
                FunctionCallback callback) {
    recv->Set(String::NewFromUtf8(isolate, name, 
        NewStringType::kNormal).ToLocalChecked(), 
        FunctionTemplate::New(isolate, callback));
}

// 设置对象的属性，属性为非函数
void Deer::Util::setObjectTemplateValue(Isolate *isolate, 
                Local<ObjectTemplate> 
                recv, const char *name, 
                // TEMPLATE OR PRIVATE
                Local<Data> value) {
    recv->Set(String::NewFromUtf8(isolate, name, 
        NewStringType::kNormal).ToLocalChecked(), 
        value);
}

// 设置对象的属性，属性为非函数
void Deer::Util::setObjectValue(Isolate *isolate, 
                Local<Object> 
                recv, const char *name, 
                Local<Value> value) {
    recv->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, name, 
        NewStringType::kNormal).ToLocalChecked(), 
        value);
}

char * Deer::Util::GetErrorByErrno() {
    return strerror(errno);
}

void Deer::Util::GetError(V8_ARGS) {
    V8_ISOLATE
    const char * error = Deer::Util::GetErrorByErrno();
    V8_RETURN(newStringToLcal(isolate, error));
}

void Deer::Util::Init(Isolate* isolate, Local<Object> target) {
    Local<ObjectTemplate> util = ObjectTemplate::New(isolate);
    Deer::Util::setMethod(isolate, util, "getErrorByErrno", Deer::Util::GetError);
    Deer::Util::setObjectValue(isolate, target, "util", util->NewInstance(isolate->GetCurrentContext()).ToLocalChecked());
}

Local<v8::FunctionTemplate> Deer::Util::NewFunctionTemplate(
            v8::Isolate* isolate,
            v8::FunctionCallback callback,
            Local<v8::Signature> signature,
            v8::ConstructorBehavior behavior,
            v8::SideEffectType side_effect_type,
            const v8::CFunction* c_function) {
        return v8::FunctionTemplate::New(isolate,
                                        callback,
                                        Local<v8::Value>(),
                                        signature,
                                        0,
                                        behavior,
                                        side_effect_type,
                                        c_function);
}

void Deer::Util::SetMethod(Local<v8::Context> context,
               Local<v8::Object> that,
               const char* name,
               v8::FunctionCallback callback) {
  Isolate* isolate = context->GetIsolate();
  Local<v8::Function> function =
      NewFunctionTemplate(isolate,
                          callback,
                          Local<v8::Signature>(),
                          v8::ConstructorBehavior::kThrow,
                          v8::SideEffectType::kHasSideEffect)
          ->GetFunction(context)
          .ToLocalChecked();
  // kInternalized strings are created in the old space.
  const v8::NewStringType type = v8::NewStringType::kInternalized;
  Local<v8::String> name_string =
      v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
  that->Set(context, name_string, function).Check();
  function->SetName(name_string);  // NODE_SET_METHOD() compatibility.
}

void Deer::Util::SetFastMethod(Local<v8::Context> context,
                   Local<v8::Object> that,
                   const char* name,
                   v8::FunctionCallback slow_callback,
                   const v8::CFunction* c_function) {
  Isolate* isolate = context->GetIsolate();
  Local<v8::Function> function =
      NewFunctionTemplate(isolate,
                          slow_callback,
                          Local<v8::Signature>(),
                          v8::ConstructorBehavior::kThrow,
                          v8::SideEffectType::kHasNoSideEffect,
                          c_function)
          ->GetFunction(context)
          .ToLocalChecked();
  const v8::NewStringType type = v8::NewStringType::kInternalized;
  Local<v8::String> name_string =
      v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
  that->Set(context, name_string, function).Check();
}

void Deer::Util::SetMethodNoSideEffect(Local<v8::Context> context,
                           Local<v8::Object> that,
                           const char* name,
                           v8::FunctionCallback callback) {
  Isolate* isolate = context->GetIsolate();
  Local<v8::Function> function =
      NewFunctionTemplate(isolate,
                          callback,
                          Local<v8::Signature>(),
                          v8::ConstructorBehavior::kThrow,
                          v8::SideEffectType::kHasNoSideEffect)
          ->GetFunction(context)
          .ToLocalChecked();
  // kInternalized strings are created in the old space.
  const v8::NewStringType type = v8::NewStringType::kInternalized;
  Local<v8::String> name_string =
      v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
  that->Set(context, name_string, function).Check();
  function->SetName(name_string);  // NODE_SET_METHOD() compatibility.
}

void Deer::Util::SetProtoMethod(v8::Isolate* isolate,
                    Local<v8::FunctionTemplate> that,
                    const char* name,
                    v8::FunctionCallback callback) {
  Local<v8::Signature> signature = v8::Signature::New(isolate, that);
  Local<v8::FunctionTemplate> t =
      NewFunctionTemplate(isolate,
                          callback,
                          signature,
                          v8::ConstructorBehavior::kThrow,
                          v8::SideEffectType::kHasSideEffect);
  // kInternalized strings are created in the old space.
  const v8::NewStringType type = v8::NewStringType::kInternalized;
  Local<v8::String> name_string =
      v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
  that->PrototypeTemplate()->Set(name_string, t);
  t->SetClassName(name_string);  // NODE_SET_PROTOTYPE_METHOD() compatibility.
}

void Deer::Util::SetProtoMethodNoSideEffect(v8::Isolate* isolate,
                                Local<v8::FunctionTemplate> that,
                                const char* name,
                                v8::FunctionCallback callback) {
  Local<v8::Signature> signature = v8::Signature::New(isolate, that);
  Local<v8::FunctionTemplate> t =
      NewFunctionTemplate(isolate,
                          callback,
                          signature,
                          v8::ConstructorBehavior::kThrow,
                          v8::SideEffectType::kHasNoSideEffect);
  // kInternalized strings are created in the old space.
  const v8::NewStringType type = v8::NewStringType::kInternalized;
  Local<v8::String> name_string =
      v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
  that->PrototypeTemplate()->Set(name_string, t);
  t->SetClassName(name_string);  // NODE_SET_PROTOTYPE_METHOD() compatibility.
}

void Deer::Util::SetInstanceMethod(v8::Isolate* isolate,
                       Local<v8::FunctionTemplate> that,
                       const char* name,
                       v8::FunctionCallback callback) {
  Local<v8::Signature> signature = v8::Signature::New(isolate, that);
  Local<v8::FunctionTemplate> t =
      NewFunctionTemplate(isolate,
                          callback,
                          signature,
                          v8::ConstructorBehavior::kThrow,
                          v8::SideEffectType::kHasSideEffect);
  // kInternalized strings are created in the old space.
  const v8::NewStringType type = v8::NewStringType::kInternalized;
  Local<v8::String> name_string =
      v8::String::NewFromUtf8(isolate, name, type).ToLocalChecked();
  that->InstanceTemplate()->Set(name_string, t);
  t->SetClassName(name_string);
}

void Deer::Util::SetConstructorFunction(Local<v8::Context> context,
                            Local<v8::Object> that,
                            const char* name,
                            Local<v8::FunctionTemplate> tmpl,
                            SetConstructorFunctionFlag flag) {
  Isolate* isolate = context->GetIsolate();
  SetConstructorFunction(
      context, that, OneByteString(isolate, name), tmpl, flag);
}

void Deer::Util::SetConstructorFunction(Local<v8::Context> context,
                            Local<v8::Object> that,
                            Local<v8::String> name,
                            Local<v8::FunctionTemplate> tmpl,
                            SetConstructorFunctionFlag flag) {
  if ((flag == SetConstructorFunctionFlag::SET_CLASS_NAME))
    tmpl->SetClassName(name);
  that->Set(context, name, tmpl->GetFunction(context).ToLocalChecked()).Check();
}

inline v8::Local<v8::String> Deer::Util::OneByteString(v8::Isolate* isolate,
                                           const char* data,
                                           int length) {
  return v8::String::NewFromOneByte(isolate,
                                    reinterpret_cast<const uint8_t*>(data),
                                    v8::NewStringType::kNormal,
                                    length).ToLocalChecked();
}

inline v8::Local<v8::String> Deer::Util::OneByteString(v8::Isolate* isolate,
                                           const signed char* data,
                                           int length) {
  return v8::String::NewFromOneByte(isolate,
                                    reinterpret_cast<const uint8_t*>(data),
                                    v8::NewStringType::kNormal,
                                    length).ToLocalChecked();
}

inline v8::Local<v8::String> Deer::Util::OneByteString(v8::Isolate* isolate,
                                           const unsigned char* data,
                                           int length) {
  return v8::String::NewFromOneByte(
             isolate, data, v8::NewStringType::kNormal, length)
      .ToLocalChecked();
}