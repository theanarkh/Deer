#ifndef MACRO_H
#define MACRO_H

#include <stddef.h>

#define V8_ARGS const FunctionCallbackInfo<Value> &args
#define V8_ISOLATE Isolate * isolate = args.GetIsolate();
#define V8_CONTEXT Local<Context> context = args.GetIsolate()->GetCurrentContext();
#define V8_RETURN(ret) args.GetReturnValue().Set(ret);
#define DEFIND_FUNC(FUNC) void FUNC(V8_ARGS);
#define container_of(ptr, type, member) ((type *) ((char *) (ptr) - offsetof(type, member)))
#endif
