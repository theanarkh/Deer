#ifndef FS_H
#define FS_H

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <cstdlib>

#include "util.h"
#include "include/common.h"
#include "env.h"
#include "base_object.h"
#include "threadpool.h"
#include "async.h"

using namespace v8;
using namespace Deer::Util;
using namespace Deer::Env;

namespace Deer {
    namespace FS {

        typedef void (*request_cb)(void *);

        class FileRequest: public Deer::Async {
            public:
                FileRequest(Environment* env, Local<Object> object);
                thread_pool_request* get_request() {
                    return &request;
                }
            private:
                thread_pool_request request;
        };

        struct OpenRequestContext {
            OpenRequestContext(std::string _path, int _flags, mode_t _mode): path(_path), flags(_flags), mode(_mode) {}
            std::string path;
            int flags;
            mode_t mode;
        };
        void OpenSync(V8_ARGS);
        void Open(V8_ARGS);
        void Init(Isolate* isolate, Local<Object> target);
    }
}

#endif
