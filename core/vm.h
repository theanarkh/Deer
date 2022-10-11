#ifndef VM_H
#define VM_H

#include "include/common.h"
#include "util.h"
#include <vector>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>

using namespace v8;
using namespace Deer::Util;

enum MODE {
    CODE_MODE = 1,
    FILE_MODE = 2
};

namespace Deer {
    namespace VM {
        void Run(V8_ARGS);
        void CompileFunction(V8_ARGS);
        void Init(Isolate* isolate, Local<Object> target);
    }
}
#endif 
