#include <node_mbgl/map.hpp>
#include <node_mbgl/display.hpp>
#include <node_mbgl/load_worker.hpp>
#include <node_mbgl/render_worker.hpp>

namespace node_mbgl
{

v8::Persistent<v8::FunctionTemplate> Map::constructor;

Map::Map()
    : node::ObjectWrap(),
    view_(display_) {
    map_ = std::make_shared<mbgl::Map>(view_);
}

Map::~Map() {}

NAN_METHOD(Map::New) {
    NanScope();

    if (!args.IsConstructCall())
    {
        NanThrowError("Cannot call constructor as function, you need to use 'new' keyword");
        NanReturnUndefined();
    }
}

NAN_METHOD(Map::load) {
    NanScope();

    if (!args[0]->IsObject())
    {
        NanThrowTypeError("first argument must be a style object");
        NanReturnUndefined();
    }
    
    if (!args[1]->IsObject())
    {
        NanThrowTypeError("second argument must be an options object");
        NanReturnUndefined();
    }

    if (!args[2]->IsString())
    {
        NanThrowTypeError("third argument must be a directory string");
        NanReturnUndefined();
    }

    const std::string base_directory(*NanUtf8String(args[2].As<v8::String>()));

    if (!args[3]->IsFunction())
    {
        NanThrowTypeError("fourth argument must be a callback");
        NanReturnUndefined();
    }

    NanCallback *callback = new NanCallback(args[3].As<v8::Function>());

    NanAsyncQueueWorker(new LoadWorker(this,
                                       args[0],
                                       args[1]->ToObject(),
                                       base_directory,
                                       callback));

    NanReturnUndefined();
}

NAN_METHOD(Map::render) {
    NanScope();

    if (!args[0]->IsFunction())
    {
        NanThrowTypeError("first argument must be a callback");
        NanReturnUndefined();
    }

    NanCallback *callback = new NanCallback(args[0].As<v8::Function>());

    NanAsyncQueueWorker(new RenderWorker(this,
                                         callback));

    NanReturnUndefined();
}

void Map::resize(unsigned int width,
                 unsigned int height,
                 unsigned int pixelRatio) {
    view_.resize(width, height, pixelRatio);
    map_->resize(width, height, pixelRatio);

    width_ = width * pixelRatio;
    height_ = height * pixelRatio;
}

}
