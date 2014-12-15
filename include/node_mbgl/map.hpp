#ifndef NODE_MBGL_MAP
#define NODE_MBGL_MAP

#include <nan.h>

#include <mbgl/map/map.hpp>
#include <mbgl/util/std.hpp>
#include <mbgl/platform/default/headless_display.hpp>
#include <mbgl/storage/caching_http_file_source.hpp>

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

typedef std::shared_ptr<mbgl::Map> mapPtr;
typedef std::shared_ptr<mbgl::CachingHTTPFileSource> fileSourcePtr;

class Map : public node::ObjectWrap
{

public:
    static void Init(v8::Handle<v8::Object> exports);
    static NAN_METHOD(Load);
    static NAN_METHOD(Render);

    void Resize(const unsigned int width,
                const unsigned int height,
                const float ratio);

    std::unique_ptr<uint32_t[]> ReadPixels();

    inline mapPtr get() { return map_; }
    inline fileSourcePtr GetFileSource() { return fileSource_; }

    inline void _Ref() { Ref(); }
    inline void _Unref() { Unref(); }

private:
    Map();
    ~Map();

    static v8::Persistent<v8::Function> constructor;
    static NAN_METHOD(New);
    static NAN_METHOD(NewInstance);

    static const std::string StringifyStyle(v8::Handle<v8::Value> styleHandle);
    static const RenderOptions* ParseOptions(v8::Local<v8::Object> obj);

    mbgl::HeadlessView view_;
    fileSourcePtr fileSource_;
    mapPtr map_;
};

} // ns node_mbgl

#endif // NODE_MBGL_MAP
