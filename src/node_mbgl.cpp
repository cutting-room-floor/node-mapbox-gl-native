#include <node.h>
#include <nan.h>

#include <node_mbgl/display.hpp>
#include <node_mbgl/render.hpp>

namespace node_mbgl
{

std::shared_ptr<mbgl::HeadlessDisplay> display_;

void Init(v8::Handle<v8::Object> exports) {
    display_ = std::make_shared<mbgl::HeadlessDisplay>();

    exports->Set(NanNew("render"),
        NanNew<v8::FunctionTemplate>(Render)->GetFunction());
    exports->Set(NanNew("renderSync"),
        NanNew<v8::FunctionTemplate>(RenderSync)->GetFunction());
}

NODE_MODULE(mapbox_gl_native, Init);

} // ns node_mbgl
