#include "fs.h"
namespace Deer {
  namespace FS {

    FileRequest::FileRequest(Environment* env, Local<Object> object): Deer::Async(env, object) {

    }

    void OpenSync(V8_ARGS) {
      V8_ISOLATE
      String::Utf8Value filename(isolate, args[0]);
      int argc = args.Length();
      int flags = O_RDONLY;
      if (argc > 1) flags = Local<Integer>::Cast(args[1])->Value();
      int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
      if (argc > 2) mode = Local<Integer>::Cast(args[2])->Value();
      Local<Integer> ret = Integer::New(isolate, open(*filename, flags, mode));
      V8_RETURN(ret);
    }

    void NewFileRequest(V8_ARGS) {
      V8_ISOLATE
      Environment* env = Environment::GetEnvByContext(isolate->GetCurrentContext());
      new FileRequest(env, args.This());
    }

    void Open(V8_ARGS) {
      V8_ISOLATE
      V8_CONTEXT
      String::Utf8Value path(isolate, args[0]);
      int argc = args.Length();
      int flags = O_RDONLY;
      // TODO
      // if (argc > 1) flags = Local<Integer>::Cast(args[1])->Value();
      mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
      // if (argc > 2) mode = Local<Integer>::Cast(args[2])->Value();
      
      Local<Object> request_ctx = args[1].As<Object>();
      Environment *env = Environment::GetEnvByContext(context);
      OpenRequestContext *ctx = new OpenRequestContext(*path, flags, mode);
      FileRequest * file_request = Deer::BaseObject::unwrap<FileRequest>(request_ctx);
      thread_pool_request* request = file_request->get_request();
      request->data = ctx;
      request->work_fn = [](thread_pool_request* request) {
        printf("open file");
      };
      ThreadPool::default_thread_pool.submit(request);
    }

    static void InitConstant(Isolate* isolate, Local<ObjectTemplate> target) {
      
      Local<ObjectTemplate> constant = ObjectTemplate::New(isolate);
      Local<ObjectTemplate> mode = ObjectTemplate::New(isolate);
      Local<ObjectTemplate> flag = ObjectTemplate::New(isolate);

      #define MODE_LIST(Set) \
        Set(S_ISUID) \
        Set(S_ISGID) \
        Set(S_ISVTX) \
        Set(S_IRUSR) \
        Set(S_IWUSR) \
        Set(S_IXUSR) \
        Set(S_IRGRP) \
        Set(S_IWGRP) \
        Set(S_IXGRP) \
        Set(S_IROTH) \
        Set(S_IWOTH) \
        Set(S_IXOTH) 
        #define Set(val) setObjectTemplateValue(isolate, mode, #val, Number::New(isolate, val));
            MODE_LIST(Set)
        #undef Set
      #undef MODE_LIST

      setObjectTemplateValue(isolate, constant, "MODE", mode);
      setObjectTemplateValue(isolate, target, "constant", constant);
    }
    void Init(Isolate* isolate, Local<Object> target) {
      Local<Context> context = isolate->GetCurrentContext();
      
      Local<ObjectTemplate> fs = ObjectTemplate::New(isolate);
      setMethod(isolate, fs, "openSync", OpenSync);
      setMethod(isolate, fs, "open", Open);
      InitConstant(isolate, fs);
      Local<FunctionTemplate> file_request = NewFunctionTemplate(isolate, NewFileRequest);
      file_request->InstanceTemplate()->SetInternalFieldCount(1);
      
      // TODO
      Local<Value> instance = fs->NewInstance(context).ToLocalChecked();
      SetConstructorFunction(context, instance.As<Object>(), "FileRequest", file_request);
      setObjectValue(isolate, target, "fs", instance);
    }
  }
}
