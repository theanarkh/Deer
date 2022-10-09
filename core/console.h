#ifndef CONSOLE_H
#define CONSOLE_H

#include "util.h"
#include "include/common.h"

using namespace v8;
using namespace Deer::Util;

namespace Deer {
    namespace Console {
        void log(V8_ARGS);
        void Init(Isolate* isolate, Local<Object> target);
    }
}

#endif