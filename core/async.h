#ifndef ASYNC_H
#define ASYNC_H

#include "util.h"
#include "env.h"
#include "include/common.h"
#include "base_object.h"

using namespace v8;
using namespace Deer::Util;
using namespace Deer::Env;

namespace Deer {
    class Async: public Deer::BaseObject {
        public:
            Async(Environment* env, Local<Object> object);
            void makeCallback(char* name, int argc = 0, v8::Local<v8::Value>* argv = nullptr);
    };
}

#endif