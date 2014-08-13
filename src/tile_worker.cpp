#include <node_mbgl/tile_worker.hpp>

#include <headless_view.hpp>
#include <mbgl/map/map.hpp>

namespace node_mbgl
{

TileWorker::TileWorker(const std::string &style,
                       const std::string &tile,
                       NanCallback *callback)
    : NanAsyncWorker(callback),
      style(style),
      tile(tile) {}

TileWorker::~TileWorker() {}

void TileWorker::Execute() {
    // Setup OpenGL
    mbgl::HeadlessView view;
    mbgl::Map map(view);

    /*
    map.setStyleJSON(style);
    map.setAppliedClasses(classes);

    view.resize(width, height);
    map.resize(width, height);
    map.setLonLatZoom(longitude, latitude, zoom);
    map.setBearing(bearing);

    // Run the loop. It will terminate when we don't have any further listeners.
    map.run();

    const std::unique_ptr<uint32_t[]> pixels(new uint32_t[width * height]);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.get());

    image = util::compress_png(width, height, pixels.get(), true);
    */

    image = "Hello world.";
}

void TileWorker::HandleOKCallback() {
    NanScope();

    v8::Local<v8::Value> argv[] = {
        NanNull(),
        NanNew<v8::String>(image)
    };

    callback->Call(2, argv);
};

} // ns node_mbgl
