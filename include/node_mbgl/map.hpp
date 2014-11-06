#ifndef NODE_MBGL_MAP
#define NODE_MBGL_MAP

#include <node.h>
#include <nan.h>

#include <mbgl/map/map.hpp>
#include <mbgl/util/std.hpp>
#include <mbgl/platform/default/headless_display.hpp>

namespace node_mbgl
{

struct LoadOptions {
    double zoom;
    double bearing;
    double latitude;
    double longitude;
    unsigned int width;
    unsigned int height;
    unsigned int pixelRatio;
    std::string accessToken;
    std::vector<std::string> classes;
};

typedef std::shared_ptr<mbgl::Map> map_ptr;

class Map : public node::ObjectWrap
{

public:
    static void Init(v8::Handle<v8::Object>);

    void Resize(unsigned int width,
                unsigned int height,
                unsigned int pixelRatio);

    inline map_ptr get() { return map_; }

    inline unsigned int getWidth() { return width_; }
    inline unsigned int getHeight() { return height_; }

    inline unsigned int* readPixels() { return view_.readPixels().get(); }

private:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static v8::Persistent<v8::Object> pmap;

    Map();
    ~Map();

    static NAN_METHOD(New);

    static const std::string StringifyStyle(v8::Handle<v8::Value> style_handle);
    static const LoadOptions* ParseOptions(v8::Local<v8::Object> options);

    static NAN_METHOD(Load);
    static NAN_METHOD(Render);

    mbgl::HeadlessView view_;
    map_ptr map_;

    unsigned int width_;
    unsigned int height_;
};

} // ns node_mbgl

#endif // NODE_MBGL_MAP
