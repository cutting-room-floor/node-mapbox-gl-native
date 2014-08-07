#include <node_mbgl/render_tile.hpp>
#include <node_mbgl/tile_worker.hpp>

namespace node_mbgl
{

NAN_METHOD(RenderTile) {
    NanScope();

    int x = args[0].As<v8::Number>()->IntegerValue();
    int y = args[1].As<v8::Number>()->IntegerValue();
    int z = args[2].As<v8::Number>()->IntegerValue();

    NanCallback *callback = new NanCallback(args[3].As<v8::Function>());

    NanAsyncQueueWorker(new TileWorker(x, y, z, callback));
    NanReturnUndefined();
};

} // ns node_mbgl
