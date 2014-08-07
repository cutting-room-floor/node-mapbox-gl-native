#ifndef NODE_MBGL_TILE_WORKER_HPP
#define NODE_MBGL_TILE_WORKER_HPP

#include <node.h>
#include <nan.h>

#include <string>

namespace node_mbgl
{

class TileWorker : public NanAsyncWorker {
public:
    TileWorker(int x, int y, int z, NanCallback *callback);
    ~TileWorker();

    void Execute();
    void HandleOKCallback();

private:
    int x;
    int y;
    int z;
    std::string tile;
};

} // ns node_mbgl

#endif // NODE_MBGL_TILE_WORKER_HPP
