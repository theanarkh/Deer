#ifndef NET_H
#define NET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "util.h"
#include "include/common.h"
#include "env.h"
// #include "io.h"
// #include "async.h"
#include "string.h"
#include "async.h"
// using namespace Deer::Env;
using namespace v8;
using namespace Deer::Util;
// using namespace Deer::Async;

namespace Deer {
    class Socket: public Deer::Async {
        public:
            Socket(Deer::Env::Environment* env, Local<Object> object);
            static void New(V8_ARGS);
            static void Init(Isolate* isolate, Local<Object> target);
            static void Bind(V8_ARGS);
            static void Connect(V8_ARGS);
            static void Setsockopt(V8_ARGS);
            static void Read(V8_ARGS);
            static void Write(V8_ARGS);
            static void Listen(V8_ARGS);
            static void Accept(V8_ARGS);
            static void Close(V8_ARGS);
            TCPHandle handle;
            // int domain;
            // int type;
    };
}
#endif 
