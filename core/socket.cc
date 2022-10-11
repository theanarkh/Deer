#include "socket.h"      

namespace Deer {

  Socket::Socket(Deer::Env::Environment* env, Local<Object> object): Deer::Async(env, object) {
      handle.data = this;
  }

  static struct sockaddr_in HandleAddrInfo(V8_ARGS) {
      V8_ISOLATE
      String::Utf8Value ip(isolate, args[0]); 
      int port = args[1].As<Integer>()->Value(); 
      struct sockaddr_in serv_addr;
      memset(&serv_addr, 0, sizeof(serv_addr));
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = inet_addr(*ip);
      serv_addr.sin_port = htons(port);
      return serv_addr;
  }

  void Socket::Bind(V8_ARGS) {   
      V8_ISOLATE
      Socket * socket_ = Deer::BaseObject::unwrap<Socket>(args.Holder());
      int fd = socket(AF_INET, SOCK_STREAM, 0);
      socket_->handle.watcher.fd = fd;
      struct sockaddr_in server_addr_info = HandleAddrInfo(args);
      int ret = bind(fd, (struct sockaddr*)&server_addr_info, sizeof(server_addr_info));
      V8_RETURN(Integer::New(isolate, ret));
  }

  void Socket::Connect(V8_ARGS) {  
      // V8_ISOLATE
      // int fd = args[0].As<Integer>()->Value();
      // struct sockaddr_in peer_addr_info = HandleAddrInfo(args);
      // V8_CONTEXT
      // Environment *env = Environment::GetEnvByContext(context);
      // struct io_uring_info *io_uring_data = env->GetIOUringData();
      // // 申请内存
      // struct connect_request *req = (struct connect_request *)malloc(sizeof(struct connect_request));
      // // to do
      // memset(req, 0, sizeof(*req));
      // req->fd = fd;
      // req->addr = (struct sockaddr *)&peer_addr_info;
      // req->addrlen = sizeof(peer_addr_info);
      // req->op = IORING_OP_CONNECT;
      // req->cb = makeCallback<onconnect>;
      // if (args.Length() > 3 && args[3]->IsFunction()) {
      //     Local<Object> obj = Object::New(isolate);
      //     Local<String> event = newStringToLcal(isolate, onconnect);
      //     obj->Set(context, event, args[3].As<Function>());
    //     req->data = (void *)new RequestContext(env, obj);
      // } else {
      //     req->data = (void *)new RequestContext(env, Local<Function>());
      // }
      // SubmitRequest((struct request*)req, io_uring_data);
  }

  void Socket::Setsockopt(V8_ARGS) {    
      int fd = args[0].As<Integer>()->Value();
      int level = args[1].As<Integer>()->Value();
      int name = args[2].As<Integer>()->Value();
      int value = args[3].As<Integer>()->Value();
      socklen_t len = sizeof(value);
      setsockopt(fd, level, name, (void *)&value, len);
  }

  void Socket::Listen(V8_ARGS) {
      V8_ISOLATE
      V8_CONTEXT
      Socket * socket = Deer::BaseObject::unwrap<Socket>(args.Holder());
      int fd = socket->handle.watcher.fd; 
      int backlog = args[0].As<Integer>()->Value(); 
      int ret = listen(fd, backlog);
      if (ret == -1) {
          V8_RETURN(Integer::New(isolate, ret));
          return;
      }
      int on = 1;
      setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
      socket->handle.watcher.event = EVFILT_READ;
      socket->handle.watcher.flags = EV_ADD;
      Local<String> event = newStringToLcal(isolate, "onconnection");
      socket->object()->Set(context, event, args[1].As<Function>());
      socket->handle.watcher.handler = [](struct io_watcher* watcher) {
        TCPHandle* handle = container_of(watcher, TCPHandle, watcher);
        Socket * server_socket = static_cast<Socket *>(handle->data);
        Environment* env = server_socket->env();
        Isolate* isolate = env->GetIsolate();
        Local<Context> context = env->GetContext();
        v8::HandleScope handle_scope(isolate);
        Context::Scope context_scope(context);
        Local<v8::FunctionTemplate> function_template = server_socket->env()->get_tcp_template();
        Local<Function> constructor = function_template->GetFunction(context).ToLocalChecked();
        Local<Object> client = constructor->NewInstance(context).ToLocalChecked();
        Socket * client_socket = Deer::BaseObject::unwrap<Socket>(client);
        int client_fd = accept(server_socket->handle.watcher.fd, nullptr, nullptr);
        client_socket->handle.watcher.fd = client_fd; 
        v8::Local<v8::Value> client_obj = client;
        v8::Local<v8::Value> argv[] = {
          client_obj
        };
        server_socket->makeCallback("onconnection", 1, argv);
      };
      Deer::Env::Environment* env = Deer::Env::Environment::GetEnvByContext(isolate->GetCurrentContext());
      struct event_loop* loop = env->get_loop();
      loop->io_watchers.push_back(&socket->handle.watcher);
      V8_RETURN(Integer::New(isolate, 0));
  }

  void Socket::Accept(V8_ARGS) {
      V8_ISOLATE
      Socket * socket = Deer::BaseObject::unwrap<Socket>(args.Holder());
      int clientFd = accept(socket->handle.watcher.fd, nullptr, nullptr);
      V8_RETURN(Integer::New(isolate, clientFd));;
  }

  void Socket::Read(V8_ARGS) {
    V8_ISOLATE
    V8_CONTEXT
    Socket * socket = Deer::BaseObject::unwrap<Socket>(args.Holder());
    socket->handle.watcher.event |= EVFILT_READ;
    socket->handle.watcher.flags = EV_DELETE;
    Local<String> event = newStringToLcal(isolate, "onread");
    socket->object()->Set(context, event, args[0].As<Function>());
    socket->handle.watcher.handler = [](struct io_watcher* watcher) {   
      TCPHandle* handle = container_of(watcher, TCPHandle, watcher);
      Socket * socket = static_cast<Socket *>(handle->data);
      Environment* env = socket->env();
      Isolate* isolate = env->GetIsolate();
      Local<Context> context = env->GetContext();
      v8::HandleScope handle_scope(isolate);
      Context::Scope context_scope(context);
      std::unique_ptr<BackingStore> bs = ArrayBuffer::NewBackingStore(isolate, 1024);
      Local<ArrayBuffer> ab = ArrayBuffer::New(isolate, std::move(bs));
      std::shared_ptr<BackingStore> backing = ab->GetBackingStore();
      int bytes = read(socket->handle.watcher.fd, backing->Data(), backing->ByteLength());
      v8::Local<v8::Value> argv[] = {
        Number::New(isolate, bytes),
        Uint8Array::New(ab, 0, bytes)
      };
      socket->makeCallback("onread", 2, argv);
    };
    Deer::Env::Environment* env = Deer::Env::Environment::GetEnvByContext(isolate->GetCurrentContext());
    struct event_loop* loop = env->get_loop();
    loop->io_watchers.push_back(&socket->handle.watcher);
  }

  void Socket::Write(V8_ARGS) { 
    V8_ISOLATE
    V8_CONTEXT
    Local<Uint8Array> uint8Array = args[0].As<Uint8Array>();
    Local<ArrayBuffer> arrayBuffer = uint8Array->Buffer();
    std::shared_ptr<BackingStore> backing = arrayBuffer->GetBackingStore();
    Environment *env = Environment::GetEnvByContext(context);
    Socket * socket = Deer::BaseObject::unwrap<Socket>(args.Holder());
    int fd = socket->handle.watcher.fd; 
    // TODO
    int bytes = write(fd, backing->Data(), backing->ByteLength());
    V8_RETURN(Number::New(isolate, bytes));
  }

  void Socket::Close(V8_ARGS) {
    V8_ISOLATE
    V8_CONTEXT
    Socket * socket = Deer::BaseObject::unwrap<Socket>(args.Holder());
    int fd = socket->handle.watcher.fd;
    int ret =  close(fd);
    if (args.Length() > 0) {
      Local<String> event = newStringToLcal(isolate, "onclose");
      socket->object()->Set(context, event, args[0].As<Function>());
    }
    socket->handle.watcher.event = EV_DELETE;
    socket->handle.watcher.handler = [](struct io_watcher* watcher) {
      TCPHandle* handle = container_of(watcher, TCPHandle, watcher);
      Socket * socket = static_cast<Socket *>(handle->data);
      Environment* env = socket->env();
      Isolate* isolate = env->GetIsolate();
      Local<Context> context = env->GetContext();
      v8::HandleScope handle_scope(isolate);
      Context::Scope context_scope(context);
      struct event_loop* loop = env->get_loop();
      std::list<struct io_watcher*>::iterator it;
      for(it = loop->io_watchers.begin(); it != loop->io_watchers.end(); it++) {
        if(*it == watcher) {
          loop->io_watchers.erase(it);
          break;
        }
      }
      socket->makeCallback("onclose", 0, nullptr);
    };
    
    V8_RETURN(Number::New(isolate, ret));
  }

  void Socket::New(V8_ARGS) {
    V8_ISOLATE
    Deer::Env::Environment* env = Deer::Env::Environment::GetEnvByContext(isolate->GetCurrentContext());
    new Socket(env, args.This());
  }
  
  void Socket::Init(Isolate* isolate, Local<Object> target) {
    Local<Object> socket = Object::New(isolate);
    Local<Object> constant = Object::New(isolate);
    Environment* env = Environment::GetEnvByContext(isolate->GetCurrentContext());
    {
      Local<Object> domain = Object::New(isolate);
      setObjectValue(isolate, domain, "AF_UNIX", Number::New(isolate, AF_UNIX));
      setObjectValue(isolate, domain, "AF_INET", Number::New(isolate, AF_INET));
      setObjectValue(isolate, domain, "AF_INET6", Number::New(isolate, AF_INET6));
      setObjectValue(isolate, constant, "domain", domain);
    }

    {
      Local<Object> type = Object::New(isolate);
      setObjectValue(isolate, type, "SOCK_STREAM", Number::New(isolate, SOCK_STREAM));
      setObjectValue(isolate, type, "SOCK_DGRAM", Number::New(isolate, SOCK_DGRAM));
      setObjectValue(isolate, type, "SOCK_RAW", Number::New(isolate, SOCK_RAW));  
      setObjectValue(isolate, constant, "type", type);
    }
    
    {
      Local<Object> level = Object::New(isolate);
      setObjectValue(isolate, level, "SOL_SOCKET", Number::New(isolate, SOL_SOCKET));
      setObjectValue(isolate, constant, "level", level);
    }

    {
      Local<Object> socket_options = Object::New(isolate);
      #define MODE_LIST(Set) \
          Set(SO_DEBUG) \
          Set(SO_REUSEADDR) \
          Set(SO_TYPE) \
          Set(SO_ERROR) \
          Set(SO_DONTROUTE) \
          Set(SO_BROADCAST) \
          Set(SO_SNDBUF) \
          Set(SO_RCVBUF) \
          Set(SO_KEEPALIVE) \
          Set(SO_OOBINLINE) \
          Set(SO_LINGER) \
          Set(SO_REUSEPORT) 
      #define Set(val) setObjectValue(isolate, socket_options, #val, Number::New(isolate, val));
          MODE_LIST(Set)
      #undef Set
    #undef MODE_LIST
      
      setObjectValue(isolate, constant, "socket_options", socket_options);
    }
    setObjectValue(isolate, socket, "constant", constant);
    Local<FunctionTemplate> t = Deer::Util::NewFunctionTemplate(isolate, New);
    env->set_tcp_template(t);
    t->InstanceTemplate()->SetInternalFieldCount(1);
    SetProtoMethod(isolate, t, "bind", Bind);
    SetProtoMethod(isolate, t, "write", Write);
    SetProtoMethod(isolate, t, "read", Read);
    SetProtoMethod(isolate, t, "listen", Listen);
    SetProtoMethod(isolate, t, "accept", Accept);
    SetProtoMethod(isolate, t, "close", Close);
    SetConstructorFunction(isolate->GetCurrentContext(), socket, "Socket", t);
    setObjectValue(isolate, target, "socket", socket);
  }
}