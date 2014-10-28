#include <node_mbgl/render_worker.hpp>

#include <mbgl/map/map.hpp>
#include <mbgl/util/image.hpp>
#include <mbgl/util/std.hpp>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "../vendor/mapbox-gl-native/common/headless_view.hpp"
#include "../vendor/mapbox-gl-native/test/fixtures/fixture_log.hpp"

#include <node_mbgl/display.hpp>

namespace node_mbgl
{

RenderWorker::RenderWorker(const std::string &style,
                           renderWorkerOptions *options,
                           const std::string &base_directory,
                           NanCallback *callback)
    : NanAsyncWorker(callback),
      style_(style),
      options_(options),
      base_directory_(base_directory) {}

RenderWorker::~RenderWorker() {}

const std::string RenderWorker::Render() {
    mbgl::HeadlessView view(display_);
    mbgl::Map map(view);

    if (!options_->accessToken.empty()) {
        map.setAccessToken(options_->accessToken);
    }

    map.setStyleJSON(style_, base_directory_);
    map.setAppliedClasses(options_->classes);

    view.resize(options_->width, options_->height, options_->pixelRatio);
    map.resize(options_->width, options_->height, options_->pixelRatio);
    map.setLonLatZoom(options_->longitude, options_->latitude, options_->zoom);
    map.setBearing(options_->bearing);

    // Run the loop. It will terminate when we don't have any further listeners.
    map.run();

    const unsigned int w = options_->width * options_->pixelRatio;
    const unsigned int h = options_->height * options_->pixelRatio;

    return mbgl::util::compress_png(w, h, view.readPixels().get(), true);
}

void RenderWorker::Execute() {
    image_ = Render();
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
