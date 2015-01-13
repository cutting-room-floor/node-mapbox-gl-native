#include <nan.h>

#include <node_mbgl/map.hpp>

namespace node_mbgl {

void InitAll(v8::Handle<v8::Object> exports) {
    Map::Init(exports);
}

NODE_MODULE(mapbox_gl_native, InitAll)

} // ns node_mbgl
