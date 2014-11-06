#include <node_mbgl/map.hpp>
#include <node_mbgl/display.hpp>
#include <node_mbgl/load_worker.hpp>
#include <node_mbgl/render_worker.hpp>

namespace node_mbgl
{

v8::Persistent<v8::FunctionTemplate> Map::constructor;

Map::Map() : node::ObjectWrap(),
    view_(display_) {
    map_ = std::make_shared<mbgl::Map>(view_);
}

Map::~Map() {}

void Map::Init(v8::Handle<v8::Object> exports) {
    NanScope();

    v8::Local<v8::FunctionTemplate> tpl = 
        NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew("Map"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "load", Load);
    NODE_SET_PROTOTYPE_METHOD(tpl, "render", Render);

    NanAssignPersistent(constructor, tpl);
    exports->Set(NanNew("Map"), tpl->GetFunction());
}

NAN_METHOD(Map::New) {
    NanScope();

    if (!args.IsConstructCall())
    {
        NanThrowError("Cannot call constructor as function, you need to use 'new' keyword");
        NanReturnUndefined();
    }

    Map *map = new Map();
    map->Wrap(args.This());
    NanReturnValue(args.This());
}

const std::string Map::StringifyStyle(v8::Handle<v8::Value> style_handle) {
    NanScope();

    v8::Handle<v8::Object> JSON = NanGetCurrentContext()->Global()->Get(NanNew("JSON"))->ToObject();
    v8::Handle<v8::Function> stringify = v8::Handle<v8::Function>::Cast(JSON->Get(NanNew("stringify")));

    return *NanUtf8String(stringify->Call(JSON, 1, &style_handle));
}

const LoadOptions* Map::ParseOptions(v8::Local<v8::Object> options) {
    NanScope();

    LoadOptions *options_ = new LoadOptions();

    options_->zoom = options->Has(NanNew("zoom")) ? options->Get(NanNew("zoom"))->NumberValue() : 0;
    options_->bearing = options->Has(NanNew("bearing")) ? options->Get(NanNew("bearing"))->NumberValue() : 0;
    options_->latitude = options->Has(NanNew("center")) ? options->Get(NanNew("center")).As<v8::Array>()->Get(0)->NumberValue() : 0;
    options_->longitude = options->Has(NanNew("center")) ? options->Get(NanNew("center")).As<v8::Array>()->Get(1)->NumberValue() : 0;
    options_->width = options->Has(NanNew("width")) ? options->Get(NanNew("width"))->IntegerValue() : 512;
    options_->height = options->Has(NanNew("height")) ? options->Get(NanNew("height"))->IntegerValue() : 512;
    options_->pixelRatio = options->Has(NanNew("pixelRatio")) ? options->Get(NanNew("pixelRatio"))->IntegerValue() : 1;

    options_->accessToken = *NanUtf8String(options->Get(NanNew("accessToken")->ToString()));

    if (options->Has(NanNew("classes"))) {
        v8::Local<v8::Array> classes(options->Get(NanNew("classes"))->ToObject().As<v8::Array>());
        int length = classes->Length();
        options_->classes.resize(length);
        for (int i = 0; i < length; i++) {
            std::string classname = *NanUtf8String(classes->Get(i)->ToString());
            options_->classes.push_back(classname);
        }
    }

    return options_;
}

NAN_METHOD(Map::Load) {
    NanScope();

    if (!args[0]->IsObject())
    {
        NanThrowTypeError("first argument must be a style object");
        NanReturnUndefined();
    }

    const std::string style(StringifyStyle(args[0]));
    
    if (!args[1]->IsObject())
    {
        NanThrowTypeError("second argument must be an options object");
        NanReturnUndefined();
    }

    const LoadOptions *options(ParseOptions(args[1]->ToObject()));

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

    Map *map = node::ObjectWrap::Unwrap<Map>(args.This());
    map->_ref();

    NanAsyncQueueWorker(new LoadWorker(map,
                                       style,
                                       options,
                                       base_directory,
                                       callback));

    NanReturnUndefined();
}

NAN_METHOD(Map::Render) {
    NanScope();

    if (!args[0]->IsFunction())
    {
        NanThrowTypeError("first argument must be a callback");
        NanReturnUndefined();
    }

    NanCallback *callback = new NanCallback(args[0].As<v8::Function>());

    Map *map = node::ObjectWrap::Unwrap<Map>(args.This());
    map->Ref();

    NanAsyncQueueWorker(new RenderWorker(map, callback));

    NanReturnUndefined();
}

void Map::Resize(unsigned int width,
                 unsigned int height,
                 unsigned int pixelRatio) {
    view_.resize(width, height, pixelRatio);
    map_->resize(width, height, pixelRatio);

    width_ = width * pixelRatio;
    height_ = height * pixelRatio;
}

}
