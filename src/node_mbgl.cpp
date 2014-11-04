#include <node.h>
#include <nan.h>

#include <node_mbgl/display.hpp>
#include <node_mbgl/map.hpp>

namespace node_mbgl
{

std::shared_ptr<mbgl::HeadlessDisplay> display_;

void Init(v8::Handle<v8::Object> exports) {
    display_ = std::make_shared<mbgl::HeadlessDisplay>();

    v8::Local<v8::FunctionTemplate> lcons = 
        NanNew<v8::FunctionTemplate>(Map::New);
    lcons->InstanceTemplate()->SetInternalFieldCount(1);
    lcons->SetClassName(NanNew("Map"));

    exports->Set(NanNew("Map"), lcons->GetFunction());
    NanAssignPersistent(Map::constructor, lcons);
}

NODE_MODULE(mapbox_gl_native, Init);

} // ns node_mbgl
