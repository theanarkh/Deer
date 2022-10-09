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

namespace Deer {
    namespace Loop {

        void init_event_system(struct event_loop* loop);

        void poll(struct event_loop* loop);

        void run_event_system(struct event_loop* loop);
        
    }
}