#ifndef Util_H
#define Util_H

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/common.h"

using namespace v8;

namespace Deer {
    namespace Util {
        char * GetErrorByErrno();
        void Log(const char * str);
        Local<String> newStringToLcal(Isolate * isolate, const char * str, NewStringType type = NewStringType::kNormal);
        MaybeLocal<String> newString(Isolate * isolate, const char * str, NewStringType type= NewStringType::kNormal);
        // 设置对象的属性，属性为函数
        void setMethod(Isolate *isolate, Local<ObjectTemplate> recv, const char *name, FunctionCallback callback);
        void setObjectTemplateValue(Isolate *isolate, Local<ObjectTemplate> recv, const char *name, Local<Data> value);
        void setObjectValue(Isolate *isolate, Local<Object> recv, const char *name, Local<Value> value);
        class PersistentToLocal {
            public:
                template <typename TypeName>
                static Local<TypeName> Strong(const PersistentBase<TypeName>& persistent) {
                    return *reinterpret_cast<Local<TypeName>*>(
                        const_cast<PersistentBase<TypeName>*>(&persistent)
                    );
                };
        };
        template <typename T>
        struct KeyCompare {
            bool operator()(T* x, T* k) const {
                return x->id >= k->id;
            }
        };

        class IdGenerator {
            public:
                IdGenerator(uint64_t _id = 0): id(_id) {}
                uint64_t getNextId() {
                    return ++id;
                }
            private:
                uint64_t id;
        };
        void GetError(V8_ARGS);
        void Init(Isolate* isolate, Local<Object> target);
        
        v8::Local<v8::FunctionTemplate> NewFunctionTemplate(
            v8::Isolate* isolate,
            v8::FunctionCallback callback,
            v8::Local<v8::Signature> signature = v8::Local<v8::Signature>(),
            v8::ConstructorBehavior behavior = v8::ConstructorBehavior::kAllow,
            v8::SideEffectType side_effect = v8::SideEffectType::kHasSideEffect,
            const v8::CFunction* c_function = nullptr);
            
        // Convenience methods for NewFunctionTemplate().
        void SetMethod(v8::Local<v8::Context> context,
                    v8::Local<v8::Object> that,
                    const char* name,
                    v8::FunctionCallback callback);

        void SetFastMethod(v8::Local<v8::Context> context,
                        v8::Local<v8::Object> that,
                        const char* name,
                        v8::FunctionCallback slow_callback,
                        const v8::CFunction* c_function);

        void SetProtoMethod(v8::Isolate* isolate,
                            v8::Local<v8::FunctionTemplate> that,
                            const char* name,
                            v8::FunctionCallback callback);

        void SetInstanceMethod(v8::Isolate* isolate,
                            v8::Local<v8::FunctionTemplate> that,
                            const char* name,
                            v8::FunctionCallback callback);

        // Safe variants denote the function has no side effects.
        void SetMethodNoSideEffect(v8::Local<v8::Context> context,
                                v8::Local<v8::Object> that,
                                const char* name,
                                v8::FunctionCallback callback);
        void SetProtoMethodNoSideEffect(v8::Isolate* isolate,
                                        v8::Local<v8::FunctionTemplate> that,
                                        const char* name,
                                        v8::FunctionCallback callback);

        enum class SetConstructorFunctionFlag {
            NONE,
            SET_CLASS_NAME,
        };

        void SetConstructorFunction(v8::Local<v8::Context> context,
                                    v8::Local<v8::Object> that,
                                    const char* name,
                                    v8::Local<v8::FunctionTemplate> tmpl,
                                    SetConstructorFunctionFlag flag =
                                        SetConstructorFunctionFlag::SET_CLASS_NAME);

        void SetConstructorFunction(v8::Local<v8::Context> context,
                                    v8::Local<v8::Object> that,
                                    v8::Local<v8::String> name,
                                    v8::Local<v8::FunctionTemplate> tmpl,
                                    SetConstructorFunctionFlag flag =
                                        SetConstructorFunctionFlag::SET_CLASS_NAME);
        
        inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
                                           const char* data,
                                           int length = -1);

        // For the people that compile with -funsigned-char.
        inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
                                                const signed char* data,
                                                int length = -1);

        inline v8::Local<v8::String> OneByteString(v8::Isolate* isolate,
                                                const unsigned char* data,
                                                int length = -1);

    }
}

#endif 
