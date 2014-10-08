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
                           const std::string &info,
                           const std::string &base_directory,
                           NanCallback *callback)
    : NanAsyncWorker(callback),
      style_(style),
      info_(info),
      base_directory_(base_directory) {}

RenderWorker::~RenderWorker() {}

const std::string RenderWorker::Render() {
    // Parse style.
    rapidjson::Document styleDoc;
    styleDoc.Parse<0>((const char *const)style_.c_str());
    // ASSERT_EQ(false, styleDoc.HasParseError());
    // ASSERT_EQ(true, styleDoc.IsObject());

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    styleDoc.Accept(writer);
    style_ = buffer.GetString();

    // Parse settings.
    rapidjson::Document value;
    value.Parse<0>((const char *const)info_.c_str());
    // ASSERT_EQ(false, infoDoc.HasParseError());
    // ASSERT_EQ(true, infoDoc.IsObject());
    
    // if (value.HasMember("center")) ASSERT_EQ(true, value["center"].IsArray());

    mbgl::Log::Set<mbgl::FixtureLogBackend>();

    const double zoom = value.HasMember("zoom") ? value["zoom"].GetDouble() : 0;
    const double bearing = value.HasMember("bearing") ? value["bearing"].GetDouble() : 0;
    const double latitude = value.HasMember("center") ? value["center"][rapidjson::SizeType(0)].GetDouble() : 0;
    const double longitude = value.HasMember("center") ? value["center"][rapidjson::SizeType(1)].GetDouble() : 0;
    const unsigned int width = value.HasMember("width") ? value["width"].GetUint() : 512;
    const unsigned int height = value.HasMember("height") ? value["height"].GetUint() : 512;
    const unsigned int pixelRatio = value.HasMember("pixelRatio") ? value["pixelRatio"].GetDouble() : 1;
    const std::string accessToken = value.HasMember("accessToken") ? std::string { value["accessToken"].GetString(), value["accessToken"].GetStringLength() } : "";
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

    mbgl::HeadlessView view(display_);
    mbgl::Map map(view);

    if (!accessToken.empty()) {
        map.setAccessToken(accessToken);
    }

    map.setStyleJSON(style_, base_directory_);
    map.setAppliedClasses(classes);

    view.resize(width, height, pixelRatio);
    map.resize(width, height, pixelRatio);
    map.setLonLatZoom(longitude, latitude, zoom);
    map.setBearing(bearing);

    // Run the loop. It will terminate when we don't have any further listeners.
    map.run();

    const unsigned int w = width * pixelRatio;
    const unsigned int h = height * pixelRatio;

    const std::unique_ptr<uint32_t[]> pixels(new uint32_t[w * h]);

    map.view.make_active();
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.get());
    map.view.make_inactive();

    return mbgl::util::compress_png(width, height, pixels.get(), true);
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
