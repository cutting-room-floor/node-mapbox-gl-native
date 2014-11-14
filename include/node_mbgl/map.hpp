#ifndef NODE_MBGL_MAP
#define NODE_MBGL_MAP

#include <nan.h>

#include <mbgl/map/map.hpp>
#include <mbgl/util/std.hpp>
#include <mbgl/platform/default/headless_display.hpp>

namespace node_mbgl
{

struct RenderOptions {
    double zoom;
    double bearing;
    double latitude;
    double longitude;
    unsigned int width;
    unsigned int height;
    float ratio;
    std::string accessToken;
    std::vector<std::string> classes;
};

typedef std::shared_ptr<mbgl::Map> map_ptr;

class Map : public node::ObjectWrap
{

public:
    static void Init(v8::Handle<v8::Object> exports);
    static NAN_METHOD(Load);
    static NAN_METHOD(Render);

    void Resize(const unsigned int width,
                const unsigned int height,
                const float ratio);

    unsigned int* ReadPixels(const unsigned int width,
                             const unsigned int height);

    inline map_ptr get() { return map_; }

    static NAN_METHOD(Set);
    static NAN_METHOD(Add);

    double Val() const { return val_; }

private:
    Map();
    ~Map();

    static v8::Persistent<v8::Function> constructor;
    static NAN_METHOD(New);
    static NAN_METHOD(NewInstance);

    static const std::string StringifyStyle(v8::Handle<v8::Value> style_handle);
    static const RenderOptions* ParseOptions(v8::Local<v8::Object> obj);

    mbgl::HeadlessView view_;
    map_ptr map_;

    double val_;
};

} // ns node_mbgl

#endif // NODE_MBGL_MAP
