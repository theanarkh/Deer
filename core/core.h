#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>  
#include "include/common.h"
#include "loader.h"
#include "process.h"
#include "socket.h"
#include "console.h"
#include "vm.h"
#include "http.h"
#include "threadpool.h"

namespace Deer {
    namespace Core {
        void register_builtins(Isolate * isolate, Local<Object> No);
    }
}