#include <node_mbgl/tile_worker.hpp>

#include <mbgl/map/map.hpp>
#include <mbgl/util/image.hpp>
#include <mbgl/util/std.hpp>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "../vendor/mapbox-gl-native/common/headless_view.hpp"

namespace node_mbgl
{

TileWorker::TileWorker(const std::string &style,
                       const std::string &info,
                       NanCallback *callback)
    : NanAsyncWorker(callback),
      style(style),
      info(info) {}

TileWorker::~TileWorker() {}

void TileWorker::Execute() {
    // Parse style.
    rapidjson::Document styleDoc;
    styleDoc.Parse<0>((const char *const)style.c_str());
    // ASSERT_EQ(false, styleDoc.HasParseError());
    // ASSERT_EQ(true, styleDoc.IsObject());

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    styleDoc.Accept(writer);
    style = buffer.GetString();

    // Parse settings.
    rapidjson::Document infoDoc;
    infoDoc.Parse<0>((const char *const)info.c_str());
    // ASSERT_EQ(false, infoDoc.HasParseError());
    // ASSERT_EQ(true, infoDoc.IsObject());
    
    const rapidjson::Value &value = infoDoc;
    // ASSERT_EQ(true, value.IsObject());
    // if (value.HasMember("center")) ASSERT_EQ(true, value["center"].IsArray());

    // Setup OpenGL
    mbgl::HeadlessView view;
    mbgl::Map map(view);

    const double zoom = value.HasMember("zoom") ? value["zoom"].GetDouble() : 0;
    const double bearing = value.HasMember("bearing") ? value["bearing"].GetDouble() : 0;
    const double latitude = value.HasMember("center") ? value["center"][rapidjson::SizeType(0)].GetDouble() : 0;
    const double longitude = value.HasMember("center") ? value["center"][rapidjson::SizeType(1)].GetDouble() : 0;
    const unsigned int width = value.HasMember("width") ? value["width"].GetUint() : 512;
    const unsigned int height = value.HasMember("height") ? value["height"].GetUint() : 512;
    std::vector<std::string> classes;
    if (value.HasMember("classes")) {
        const rapidjson::Value &js_classes = value["classes"];
        // ASSERT_EQ(true, js_classes.IsArray());
        for (rapidjson::SizeType i = 0; i < js_classes.Size(); i++) {
            const rapidjson::Value &js_class = js_classes[i];
            // ASSERT_EQ(true, js_class.IsString());
            classes.push_back({ js_class.GetString(), js_class.GetStringLength() });
        }
    }

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

    image = mbgl::util::compress_png(width, height, pixels.get(), true);
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
