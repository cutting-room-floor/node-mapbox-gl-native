#include <node_mbgl/render_worker.hpp>
#include <mbgl/util/image.hpp>

namespace node_mbgl
{

RenderWorker::RenderWorker(Map* map,
                           const RenderOptions* options,
                           NanCallback *callback)
    : NanAsyncWorker(callback),
    map_(map),
    options_(options) {
        map_->_Ref();
    }

RenderWorker::~RenderWorker() {}

void RenderWorker::Execute() {
    try {
        map_->GetFileSource()->setAccessToken(options_->accessToken);

        map_->get()->setAppliedClasses(options_->classes);

        map_->Resize(options_->width, options_->height, options_->ratio);

        map_->get()->setLonLatZoom(options_->longitude, options_->latitude, options_->zoom);
        map_->get()->setBearing(options_->bearing);

        // Run the loop. It will terminate when we don't have any further listeners.
        map_->get()->run();

        const unsigned int width = options_->width * options_->ratio;
        const unsigned int height = options_->height * options_->ratio;

        const auto pixels = map_->ReadPixels();
        image_ = mbgl::util::compress_png(width, height, pixels.get());
    } catch(const std::exception& ex) {
        SetErrorMessage(ex.what());
    }
}

void RenderWorker::HandleOKCallback() {
    NanScope();

    v8::Local<v8::Value> argv[] = {
        NanNull(),
        NanNewBufferHandle(image_.c_str(), image_.length())
    };

    callback->Call(2, argv);

    map_->_Unref();
};

} // ns node_mbgl
