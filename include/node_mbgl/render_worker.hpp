#ifndef NODE_MBGL_RENDER_WORKER
#define NODE_MBGL_RENDER_WORKER

#include <nan.h>
#include <node_mbgl/map.hpp>

namespace node_mbgl {

class RenderWorker : public NanAsyncWorker {
public:
    RenderWorker(Map *map, std::unique_ptr<RenderOptions> &&options, NanCallback *callback);
    ~RenderWorker();

    void Execute();
    void HandleOKCallback();
    void WorkComplete();

private:
    Map *map_;
    std::unique_ptr<RenderOptions> options_;
    std::string image_;
};

} // ns node_mbgl

#endif // NODE_MBGL_RENDER_WORKER
