#include <node_mbgl/render_worker.hpp>
#include <mbgl/util/image.hpp>

namespace node_mbgl {

RenderWorker::RenderWorker(Map *map, std::unique_ptr<RenderOptions> options,
                           NanCallback *callback)
    : NanAsyncWorker(callback), map_(map), options_(std::move(options)) {
    map_->Ref();
}

RenderWorker::~RenderWorker() {
    map_->Unref();
}

void RenderWorker::Execute() {
    try {
        map_->fileSource_.setAccessToken(options_->accessToken);
        map_->view_.resize(options_->width, options_->height, options_->ratio);
        map_->map_.resize(options_->width, options_->height, options_->ratio);
        map_->map_.setAppliedClasses(options_->classes);
        map_->map_.setLonLatZoom(options_->longitude, options_->latitude, options_->zoom);
        map_->map_.setBearing(options_->bearing);

        // Run the loop. It will terminate when we don't have any further listeners.
        map_->map_.run();

        const unsigned int width = options_->width * options_->ratio;
        const unsigned int height = options_->height * options_->ratio;
        image_ = mbgl::util::compress_png(width, height, map_->view_.readPixels().get());
    } catch (const std::exception &ex) {
        SetErrorMessage(ex.what());
    }
}

void RenderWorker::WorkComplete() {
    NanAsyncWorker::WorkComplete();

    // Continue processing remaining items in the queue.
    map_->ProcessNext();
}

void RenderWorker::HandleOKCallback() {
    NanScope();

    v8::Local<v8::Value> argv[] = {NanNull(), NanNewBufferHandle(image_.c_str(), image_.length())};

    callback->Call(2, argv);
};

} // ns node_mbgl
