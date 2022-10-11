#ifndef COMMON_H
#define COMMON_H

#include "v8.h"
#include "macro.h"
#include <stdio.h>
#include <vector>
#include <list>

struct io_watcher;
typedef void (*io_watcher_handler) (struct io_watcher* watcher);
struct io_watcher
{
    int fd = -1;
    int event = 0;
    int pevent = 0;
    int flags = 0;
    io_watcher_handler handler = nullptr;
};

struct Handle
{
   void* data = nullptr;
};

struct TCPHandle: public Handle
{
    io_watcher watcher;
};

struct event_loop
{
   int event_fd = -1;
   std::list<struct io_watcher*> io_watchers;
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