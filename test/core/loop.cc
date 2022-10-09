
#include "loop.h"


void Deer::Loop::init_event_system(struct event_loop* loop) {
    loop->event_fd = kqueue();
}

void Deer::Loop::poll(struct event_loop* loop) {
    struct kevent events[1024];
    struct kevent event;
    int nevent = 0;
    
    for (auto watcher: loop->io_watchers) {
        if (watcher->event != watcher->pevent) {
            EV_SET(&events[nevent++], watcher->fd, watcher->event, EV_ADD, 0, 0, (void *)watcher);
            watcher->pevent = watcher->event;
        }
    }
    struct timespec *timeout = nullptr;
	int n = kevent(loop->event_fd, events, nevent, &event, 1, timeout);
    if (n > 0) {
        io_watcher* watcher = static_cast<io_watcher *>(event.udata);
        watcher->handler(watcher);
    }
};

void Deer::Loop::run_event_system(struct event_loop* loop) {
	while(1) {
        poll(loop);
    }
};