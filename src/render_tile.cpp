#include <node_mbgl/render_tile.hpp>
#include <node_mbgl/tile_worker.hpp>

namespace node_mbgl
{

NAN_METHOD(RenderTile) {
    NanScope();

    const std::string style(*v8::String::Utf8Value(args[0].As<v8::String>()));
    const std::string info(*v8::String::Utf8Value(args[1].As<v8::String>()));
    NanCallback *callback = new NanCallback(args[2].As<v8::Function>());

    NanAsyncQueueWorker(new TileWorker(style, info, callback));
    NanReturnUndefined();
};

} // ns node_mbgl
