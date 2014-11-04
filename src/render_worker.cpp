#include <node_mbgl/render_worker.hpp>
#include <mbgl/util/image.hpp>

namespace node_mbgl
{

RenderWorker::RenderWorker(Map *map,
                           NanCallback *callback)
    : NanAsyncWorker(callback),
      map_(map) {}

RenderWorker::~RenderWorker() {}

void RenderWorker::Execute() {
    // Run the loop. It will terminate when we don't have any further listeners.
    map_->get()->run();

    image_ =  mbgl::util::compress_png(map_->getWidth(),
                                       map_->getHeight(),
                                       map_->readPixels(),
                                       true);
}

void RenderWorker::HandleOKCallback() {
    NanScope();

    v8::Local<v8::Value> argv[] = {
        NanNull(),
        NanNewBufferHandle(image_.c_str(), image_.length())
    };

    callback->Call(2, argv);
};

} // ns node_mbgl
