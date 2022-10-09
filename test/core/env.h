#ifndef ENV_H
#define ENV_H

#include <fcntl.h>
#include <unistd.h>
#include "include/common.h"
#include "util.h"
#include "loop.h"

using namespace v8;
using namespace Deer::Util;

namespace Deer {
    namespace Env {
        enum {
            CONTEXT_INDEX
        } ENV_INDEX;
        
        class Environment {
            public:
                Environment(Local<Context> context);
                static Environment * GetEnvByContext(Local<Context> context);
                Isolate * GetIsolate() const {
                    return isolate_;
                }
                Local<Context> GetContext() const {
                    return Deer::Util::PersistentToLocal::Strong(context_);
                }
                struct event_loop* get_loop() {
                    return &loop;
                }
                void setArgv(char **_argv) {
                    argv = _argv;
                }
                char ** getArgv() {
                    return argv;
                }
                void setArgc(int _argc) {
                    argc = _argc;
                }
                int getArgc() {
                    return argc;
                }
                void set_tcp_template(Local<v8::FunctionTemplate> function_template) {
                    tcp_template_.Reset(isolate_, function_template);
                }
                Local<v8::FunctionTemplate> get_tcp_template() {
                    return Deer::Util::PersistentToLocal::Strong(tcp_template_);
                }
            private:
                Global<Context> context_;
                Isolate * isolate_;
                struct event_loop loop;
                int argc;
                char **argv;
                Global<v8::FunctionTemplate> tcp_template_;
        };
    }
}

#endif

