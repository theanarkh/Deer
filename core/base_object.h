#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "util.h"
#include "include/common.h"
#include "env.h"

using namespace v8;
using namespace Deer::Util;
using namespace Deer::Env;
namespace Deer {
    class BaseObject {
        public:
            BaseObject(Environment* env, Local<Object> object);
            virtual ~BaseObject();
            Environment* env() const;
            Local<Object> object();
            void MakeWeak();
            void ClearWeak();
            template <typename T>
            static T * unwrap(Local<Object> object) {
                return static_cast<T*>(object->GetAlignedPointerFromInternalField(0));
            };
        private: 
            Environment * _env;
            Global<Object> _object;
    };
}

#endif