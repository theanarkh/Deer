#ifndef COMMON_H
#define COMMON_H

#include "v8.h"
#include "macro.h"
#include <stdio.h>
#include <vector>
#include <list>

struct io_watcher;
typedef void (*io_watcher_handler) (struct io_watcher* watcher, int events);

struct Task {

};

struct io_watcher
{
    int fd = -1;
    int event = 0;
    int pevent = 0;
    int flags = 0;
    io_watcher_handler handler = nullptr;
};

struct buffer {
    char *data = nullptr;
    int offset = 0;
    int len = 0;
};

struct BaseHandle
{
   void* data = nullptr;
};

struct event_loop
{
   int event_fd = -1;
   int event_fd_count = 0;
   io_watcher async_reader_io_watcher;
   int async_writer_fd = -1;
   std::list<io_watcher*> io_watchers;
   std::list<Task*> pending_tasks;
};

enum class RequestType {
    TCP,
};

class Request {
    public:
        RequestType type;
        struct io_watcher watcher;
};

class Buffer {
    public:
        Buffer(char * data, int len): data_(data),len_(len) {}
        char * data_;
        int len_;
};



struct TCPRequest: public Request {
    std::list<Buffer> writeQueue;
};


#endif