#ifndef NODE_MBGL_MAP
#define NODE_MBGL_MAP

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
    float ratio;
    std::string accessToken;
    std::vector<std::string> classes;
};

typedef std::shared_ptr<mbgl::Map> map_ptr;

class Map : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::Object> exports);
    static NAN_METHOD(Set);
    static NAN_METHOD(Add);
    double Val() const { return val_; }

private:
    Map();
    ~Map();

    static v8::Persistent<v8::Function> constructor;
    static NAN_METHOD(New);
    static NAN_METHOD(NewInstance);

    mbgl::HeadlessView view_;
    map_ptr map_;

    double val_;
};

} // ns node_mbgl

#endif // NODE_MBGL_MAP
