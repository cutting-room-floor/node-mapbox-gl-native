#ifndef NODE_MBGL_MAP
#define NODE_MBGL_MAP

#include <nan.h>

#include <mbgl/map/map.hpp>
#include <mbgl/util/std.hpp>
#include <mbgl/platform/default/headless_display.hpp>
#include <mbgl/storage/caching_http_file_source.hpp>

#include <queue>

namespace node_mbgl {

class RenderWorker;

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

class Map : public node::ObjectWrap {
public:
    friend RenderWorker;

    Map();

    static void Init(v8::Handle<v8::Object> exports);
    static v8::Persistent<v8::Function> constructor;
    static NAN_METHOD(New);
    static NAN_METHOD(NewInstance);
    static NAN_METHOD(Load);
    static NAN_METHOD(Render);
    static NAN_METHOD(Terminate);

    void ProcessNext();

private:
    mbgl::HeadlessView view_;
    mbgl::CachingHTTPFileSource fileSource_;
    mbgl::Map map_;

    std::queue<std::unique_ptr<RenderWorker>> queue_;
};

} // ns node_mbgl

#endif // NODE_MBGL_MAP
