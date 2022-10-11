
#include "loop.h"


void Deer::Loop::init_event_system(struct event_loop* loop) {
    loop->event_fd = kqueue();
}

int Deer::Loop::poll(struct event_loop* loop) {
    
    struct kevent events[1024];
    struct kevent ready_events[1024];
    int nevent = 0;
    
    for (auto watcher: loop->io_watchers) {
        if (watcher->event != watcher->pevent) {
            EV_SET(&events[nevent++], watcher->fd, watcher->event, watcher->flags, 0, 0, (void *)watcher);
            watcher->pevent = watcher->event;
        }
    }
    struct timespec *timeout = nullptr;
    if (nevent == 0) {
        return 0;
    }
	int n = kevent(loop->event_fd, events, nevent, ready_events, 1024, timeout);
    if (n > 0) {
        for (int i = 0; i < n; i++) {
            io_watcher* watcher = static_cast<io_watcher *>(ready_events[i].udata);
            watcher->handler(watcher);
        }
    }
    for (auto watcher: loop->io_watchers) {
        if (watcher->flags == EV_DELETE) {
            watcher->handler(watcher);
        }
    }
    return nevent;
};

void Deer::Loop::run_event_system(struct event_loop* loop) {
	while(1) {
        int event = poll(loop);
        if (event == 0) {
            break;
        }
    }
};