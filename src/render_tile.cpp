#include <node_mbgl/render_tile.hpp>

#include <mbgl/map/map.hpp>
#include <mbgl/util/image.hpp>
#include <mbgl/util/std.hpp>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "../vendor/mapbox-gl-native/common/headless_view.hpp"
#include "../vendor/mapbox-gl-native/test/fixtures/fixture_log.hpp"

#include <iostream>

namespace node_mbgl
{

NAN_METHOD(RenderTile) {
    NanScope();

    std::string style_(*v8::String::Utf8Value(args[0].As<v8::String>()));
    const std::string info_(*v8::String::Utf8Value(args[1].As<v8::String>()));
    const std::string base_directory(*v8::String::Utf8Value(args[2].As<v8::String>()));

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

    mbgl::HeadlessView view;
    mbgl::Map map(view);

    map.setStyleJSON(style_, base_directory);
    map.setAppliedClasses(classes);

    view.resize(width, height);
    map.resize(width, height);
    map.setLonLatZoom(longitude, latitude, zoom);
    map.setBearing(bearing);

    // Run the loop. It will terminate when we don't have any further listeners.
    map.run();

    const std::unique_ptr<uint32_t[]> pixels(new uint32_t[width * height]);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.get());

    const std::string image_ = mbgl::util::compress_png(width, height, pixels.get(), true);

    NanReturnValue(node::Buffer::New(image_.c_str(), image_.length())->handle_);
};

} // ns node_mbgl
