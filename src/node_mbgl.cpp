#include <node.h>
#include <nan.h>

#include <node_mbgl/render_tile.hpp>

namespace node_mbgl
{

void Init(v8::Handle<v8::Object> exports) {
    exports->Set(NanNew("renderTile"),
        NanNew<v8::FunctionTemplate>(RenderTile)->GetFunction());
}

NODE_MODULE(mapbox_gl_native, Init);

} // ns node_mbgl
