#ifndef NODE_MBGL_RENDER
#define NODE_MBGL_RENDER

#include <node.h>
#include <nan.h>

using namespace v8;

namespace node_mbgl
{

NAN_METHOD(Render);
NAN_METHOD(RenderSync);

} // ns node_mbgl

#endif // NODE_MBGL_RENDER
