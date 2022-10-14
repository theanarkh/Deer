
#include "loop.h"

void Deer::Loop::init_event_system(struct event_loop* loop) {
    loop->event_fd = kqueue();
    int fds[2];
    pipe(fds);
    loop->async_reader_io_watcher.fd = fds[0];
    // loop->io_watchers.push_back(&loop->async_reader_io_watcher);
    loop->async_writer_fd = fds[1];
}

int Deer::Loop::poll(struct event_loop* loop) {
    struct kevent events[MAX_EVENT_SIZE];
    struct kevent ready_events[MAX_EVENT_SIZE];
    int nevent = 0;
    for (auto watcher: loop->io_watchers) {
        if ((watcher->event & POLL_IN) == 0 && (watcher->pevent & POLL_IN) != 0) {
            EV_SET(&events[nevent++], watcher->fd, EVFILT_READ, EV_ADD, 0, 0, (void *)watcher);
            if (nevent == MAX_EVENT_SIZE) {
                kevent(loop->event_fd, events, nevent, 0, 0, nullptr);
                nevent = 0;
            }
        }
        if ((watcher->event & POLL_OUT) == 0 && (watcher->pevent & POLL_OUT) != 0) {
            EV_SET(&events[nevent++], watcher->fd, EVFILT_WRITE, EV_ADD, 0, 0, (void *)watcher);
            if (nevent == MAX_EVENT_SIZE) {
                kevent(loop->event_fd, events, nevent, 0, 0, nullptr);
                nevent = 0;
            }
        }
        watcher->event = watcher->pevent;
    }
    struct timespec *timeout = nullptr;
    loop->event_fd_count += nevent;
    if (loop->event_fd_count == 0) {
        return 0;
    }
	int n = kevent(loop->event_fd, events, nevent, ready_events, MAX_EVENT_SIZE, timeout);
    if (n > 0) {
        for (int i = 0; i < n; i++) {
            io_watcher* watcher = static_cast<io_watcher *>(ready_events[i].udata);
            int event = 0;
            if (watcher->pevent == 0) {
                struct kevent event[1];
                loop->event_fd_count--;
                EV_SET(&event[0], watcher->fd, ready_events[i].filter, EV_DELETE, 0, 0, nullptr);
                kevent(loop->event_fd, event, 1, nullptr, 0, nullptr);
                watcher->handler(watcher, 0);
                continue;
            } else if (ready_events[i].filter == EVFILT_READ) {
                if (watcher->pevent & POLL_IN) {
                    watcher->handler(watcher, POLL_IN);
                } else {
                    struct kevent event[1];
                    loop->event_fd_count--;
                    EV_SET(&event[0], watcher->fd, EVFILT_READ, EV_DELETE, 0, 0, nullptr);
                    kevent(loop->event_fd, event, 1, nullptr, 0, nullptr);
                }
            } else if (ready_events[i].filter == EVFILT_WRITE) {
                if (watcher->pevent & POLL_OUT) {
                    watcher->handler(watcher, POLL_OUT);
                } else {
                    struct kevent event[1];
                    loop->event_fd_count--;
                    EV_SET(&event[0], watcher->fd, EVFILT_WRITE, EV_DELETE, 0, 0, nullptr);
                    kevent(loop->event_fd, event, 1, nullptr, 0, nullptr);
                }
            }
        }
    }
    return loop->event_fd_count;
};

void Deer::Loop::run_event_system(struct event_loop* loop) {
	while(1) {
        int event = poll(loop);
        if (event == 0) {
            break;
        }
    }
};