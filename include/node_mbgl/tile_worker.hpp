#ifndef NODE_MBGL_TILE_WORKER_HPP
#define NODE_MBGL_TILE_WORKER_HPP

#include <node.h>
#include <nan.h>

#include <string>

namespace node_mbgl
{

class TileWorker : public NanAsyncWorker {
public:
    TileWorker(const std::string &style,
               const std::string &tile,
               NanCallback *callback);
    ~TileWorker();

    void Execute();
    void HandleOKCallback();

private:
    const std::string style; 
    const std::string tile; 

    std::string image;
};

} // ns node_mbgl

#endif // NODE_MBGL_TILE_WORKER_HPP
