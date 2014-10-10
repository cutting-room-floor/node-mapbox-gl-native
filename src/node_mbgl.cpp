#include <node.h>
#include <nan.h>

#include <node_mbgl/render.hpp>

namespace node_mbgl
{

void Init(v8::Handle<v8::Object> exports) {
    exports->Set(NanNew("render"),
        NanNew<v8::FunctionTemplate>(Render)->GetFunction());
    exports->Set(NanNew("renderSync"),
        NanNew<v8::FunctionTemplate>(RenderSync)->GetFunction());
}

NODE_MODULE(mapbox_gl_native, Init);

} // ns node_mbgl
