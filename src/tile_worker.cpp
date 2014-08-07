#include <node_mbgl/tile_worker.hpp>

namespace node_mbgl
{

TileWorker::TileWorker(int x, int y, int z, NanCallback *callback)
    : NanAsyncWorker(callback),
      x(x),
      y(y),
      z(z) {}

TileWorker::~TileWorker() {}

void TileWorker::Execute() {
    tile = "Hello world.";
}

void TileWorker::HandleOKCallback() {
    NanScope();

    v8::Local<v8::Value> argv[] = {
        NanNull(),
        NanNew<v8::String>(tile)
    };

    callback->Call(2, argv);
};

} // ns node_mbgl
