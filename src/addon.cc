#include <nan.h>
#include <node_mbgl/myobject.h>

void InitAll(v8::Handle<v8::Object> exports) {
  MyObject::Init(exports);
}

NODE_MODULE(addon, InitAll)
