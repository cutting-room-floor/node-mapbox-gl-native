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
               const std::string &info,
               NanCallback *callback);
    ~TileWorker();

    void Execute();
    void HandleOKCallback();

private:
    std::string style_; 
    std::string info_; 
    std::string image_;
};

} // ns node_mbgl

#endif // NODE_MBGL_TILE_WORKER_HPP
