#include <node_mbgl/render_tile.hpp>
#include <node_mbgl/tile_worker.hpp>

namespace node_mbgl
{

NAN_METHOD(RenderTile) {
    NanScope();

    const std::string style(*v8::String::Utf8Value(args[0].As<v8::String>()));

    if (!node::Buffer::HasInstance(args[1])) {
        ThrowException (v8::Exception::TypeError(
              v8::String::New("Second argument must be a buffer.")));
    } 

    char* tile(node::Buffer::Data(args[1]->ToObject()));

    NanCallback *callback = new NanCallback(args[2].As<v8::Function>());

    NanAsyncQueueWorker(new TileWorker(style, tile, callback));
    NanReturnUndefined();
};

} // ns node_mbgl
