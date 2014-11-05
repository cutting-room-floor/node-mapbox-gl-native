#ifndef NODE_MBGL_RENDER_WORKER
#define NODE_MBGL_RENDER_WORKER

#include <node.h>
#include <nan.h>

#include <node_mbgl/map.hpp>

namespace node_mbgl
{

class RenderWorker : public NanAsyncWorker {
public:
    RenderWorker(Map *map, NanCallback *callback);
    ~RenderWorker();

    void Execute();
    void HandleOKCallback();

private:
    Map *map_;
    std::string image_;
};

} // ns node_mbgl

#endif // NODE_MBGL_RENDER_WORKER
