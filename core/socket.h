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
            static void Init(Isolate* isolate, Local<Object> target);
            static DEFIND_FUNC(New);
            static DEFIND_FUNC(Bind);
            static DEFIND_FUNC(Connect);
            static DEFIND_FUNC(Setsockopt);
            static DEFIND_FUNC(Read);
            static DEFIND_FUNC(Write);
            static DEFIND_FUNC(Listen);
            static DEFIND_FUNC(Accept);
            static DEFIND_FUNC(Close);
            struct WriteRequest: public Async {
                WriteRequest(Deer::Env::Environment* env, Local<Object> object, buffer& _buf);
                ~WriteRequest() {
                    delete[] buf.data;
                }
                buffer buf;
            };
            struct TCPHandle: public BaseHandle
            {
                std::list<WriteRequest*> write_queue;
                io_watcher watcher;
            };
            TCPHandle handle;
            
            // int domain;
            // int type;
    };
}
#endif 
