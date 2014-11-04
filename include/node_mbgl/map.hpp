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
    Map();
    ~Map();

    static v8::Persistent<v8::FunctionTemplate> constructor;

    static NAN_METHOD(New);
    static NAN_METHOD(load);
    static NAN_METHOD(render);

    void resize(unsigned int width,
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
