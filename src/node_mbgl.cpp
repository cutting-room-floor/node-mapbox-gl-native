#include <node.h>
#include <nan.h>

using namespace v8;

namespace node_mbgl
{

void Init(v8::Handle<v8::Object> exports) {
   //  exports->Set(NanNew("renderTile"), NanNew<FunctionTemplate>(RenderTile)->GetFunction());
}

/*
class RenderWorker : public NanAsyncWorker {
 public:

  RenderWorker(int x, int y, int z, NanCallback *callback)
    : NanAsyncWorker(callback), x(x), y(y), z(z) {}

  ~RenderWorker() {}

  // Worker thread render stuff goes here. Only use `this` for I/O. No V8!
  void Execute () {
    printf("TODO: render a real tile\n");
  }

  void HandleOKCallback () {
    NanScope();

    char str[100];
    snprintf(str,100,"pretend this is a map tile for %d/%d/%d",x,y,z);
    Local<Value> argv[] = {
      // TODO: this should be a tile image
      NanNew<String>(str)
    };

    callback->Call(1, argv);
  }

 private:
  int x;
  int y;
  int z;
};

NAN_METHOD(RenderTile) {
  NanScope();

  int x = args[0].As<Number>()->IntegerValue();
  int y = args[1].As<Number>()->IntegerValue();
  int z = args[2].As<Number>()->IntegerValue();

  Local<Function> callback = args[3].As<Function>();
  NanCallback* nanCallback = new NanCallback(callback);
  
  RenderWorker* worker = new RenderWorker(x,y,z,nanCallback);
  NanAsyncQueueWorker(worker);
  NanReturnUndefined();
}
*/

NODE_MODULE(mbgl, Init);

} // ns node_mbgl
