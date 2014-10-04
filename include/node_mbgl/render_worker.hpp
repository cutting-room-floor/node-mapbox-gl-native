#ifndef NODE_MBGL_RENDER_WORKER
#define NODE_MBGL_RENDER_WORKER

#include <node.h>
#include <nan.h>

#include <string>

namespace node_mbgl
{

class RenderWorker : public NanAsyncWorker {
public:
    RenderWorker(const std::string &style,
                 const std::string &info,
                 const std::string &base_directory,
                 NanCallback *callback);
    ~RenderWorker();

    const std::string Render();

    void Execute();
    void HandleOKCallback();

private:
    std::string style_; 
    const std::string info_; 
    const std::string base_directory_; 
    std::string image_;
};

} // ns node_mbgl

#endif // NODE_MBGL_RENDER_WORKER
