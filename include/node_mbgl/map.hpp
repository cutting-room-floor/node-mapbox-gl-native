#ifndef NODE_MBGL_MAP
#define NODE_MBGL_MAP

#include <node.h>
#include <nan.h>

#include <mbgl/map/map.hpp>
#include <mbgl/util/std.hpp>
#include <mbgl/platform/default/headless_display.hpp>

namespace node_mbgl
{

typedef std::shared_ptr<mbgl::Map> map_ptr;

class Map : public node::ObjectWrap
{

public:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static void Init(v8::Handle<v8::Object>);

    Map();
    ~Map();

    static NAN_METHOD(New);
    NAN_METHOD(Load);
    NAN_METHOD(Render);

    void Resize(unsigned int width,
                unsigned int height,
                unsigned int pixelRatio);

    inline map_ptr get() { return map_; }

    inline unsigned int getWidth() { return width_; }
    inline unsigned int getHeight() { return height_; }

    inline unsigned int* readPixels() { return view_.readPixels().get(); }

private:
    mbgl::HeadlessView view_;
    map_ptr map_;

    unsigned int width_;
    unsigned int height_;
};

} // ns node_mbgl

#endif // NODE_MBGL_MAP
