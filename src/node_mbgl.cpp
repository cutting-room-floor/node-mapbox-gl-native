#include <nan.h>

#include <node_mbgl/display.hpp>
#include <node_mbgl/myobject.hpp>

namespace node_mbgl
{

std::shared_ptr<mbgl::HeadlessDisplay> display_;

void InitAll(v8::Handle<v8::Object> exports) {
    display_ = std::make_shared<mbgl::HeadlessDisplay>();

    MyObject::Init(exports);
}

NODE_MODULE(mapbox_gl_native, InitAll)

} // ns node_mbgl
