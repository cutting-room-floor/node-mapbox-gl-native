#include <node.h>
#include <node_mbgl/map.hpp>
#include <node_mbgl/display.hpp>
#include <node_mbgl/render_worker.hpp>

namespace node_mbgl {

Map::Map()
    : view_(display_),
      fileSource_(std::make_shared<mbgl::CachingHTTPFileSource>(
          "")), // Pass empty string to disable the cache
      map_(std::make_shared<mbgl::Map>(view_, *fileSource_)){};

Map::~Map(){};

v8::Persistent<v8::Function> Map::constructor;

void Map::Init(v8::Handle<v8::Object> exports) {
    NanScope();

    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew("Map"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "load", Load);
    NODE_SET_PROTOTYPE_METHOD(tpl, "render", Render);
    NODE_SET_PROTOTYPE_METHOD(tpl, "terminate", Terminate);

    NanAssignPersistent(constructor, tpl->GetFunction());

    exports->Set(NanNew("Map"), NanNew<v8::FunctionTemplate>(NewInstance)->GetFunction());
}

NAN_METHOD(Map::New) {
    NanScope();

    Map *map = new Map();
    map->Wrap(args.Holder());

    NanReturnHolder();
}

NAN_METHOD(Map::NewInstance) {
    NanScope();

    if (!args.IsConstructCall()) {
        NanThrowError("Cannot call constructor as function, you need to use 'new' keyword");
        NanReturnUndefined();
    }

    const unsigned argc = 0;
    v8::Local<v8::Value> argv[argc] = {};
    v8::Local<v8::Function> cons = NanNew<v8::Function>(constructor);
    v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

    NanReturnValue(instance);
}

const std::string Map::StringifyStyle(v8::Handle<v8::Value> styleHandle) {
    NanScope();

    v8::Handle<v8::Object> JSON = NanGetCurrentContext()->Global()->Get(NanNew("JSON"))->ToObject();
    v8::Handle<v8::Function> stringify = v8::Handle<v8::Function>::Cast(JSON->Get(NanNew("stringify")));

    return *NanUtf8String(stringify->Call(JSON, 1, &styleHandle));
}

const RenderOptions *Map::ParseOptions(v8::Local<v8::Object> obj) {
    NanScope();

    RenderOptions *options = new RenderOptions();

    options->zoom = obj->Has(NanNew("zoom")) ? obj->Get(NanNew("zoom"))->NumberValue() : 0;
    options->bearing = obj->Has(NanNew("bearing")) ? obj->Get(NanNew("bearing"))->NumberValue() : 0;
    options->latitude = obj->Has(NanNew("center"))
                            ? obj->Get(NanNew("center")).As<v8::Array>()->Get(0)->NumberValue()
                            : 0;
    options->longitude = obj->Has(NanNew("center"))
                             ? obj->Get(NanNew("center")).As<v8::Array>()->Get(1)->NumberValue()
                             : 0;
    options->width = obj->Has(NanNew("width")) ? obj->Get(NanNew("width"))->IntegerValue() : 512;
    options->height = obj->Has(NanNew("height")) ? obj->Get(NanNew("height"))->IntegerValue() : 512;
    options->ratio = obj->Has(NanNew("ratio")) ? obj->Get(NanNew("ratio"))->IntegerValue() : 1;

    options->accessToken = *NanUtf8String(obj->Get(NanNew("accessToken")->ToString()));

    if (obj->Has(NanNew("classes"))) {
        v8::Local<v8::Array> classes(obj->Get(NanNew("classes"))->ToObject().As<v8::Array>());
        int length = classes->Length();
        options->classes.resize(length);
        for (int i = 0; i < length; i++) {
            std::string classname = *NanUtf8String(classes->Get(i)->ToString());
            options->classes.push_back(classname);
        }
    }

    return options;
}

NAN_METHOD(Map::Load) {
    NanScope();

    if (args.Length() != 1) {
        NanThrowError("Wrong number of arguments");
        NanReturnUndefined();
    }

    std::string style;

    if (args[0]->IsObject()) {
        style = StringifyStyle(args[0]);
    } else if (args[0]->IsString()) {
        v8::Local<v8::String> toStr = args[0]->ToString();
        style.resize(toStr->Utf8Length());
        toStr->WriteUtf8(const_cast<char *>(style.data()));
    } else {
        NanThrowTypeError("First argument must be a style string or object");
        NanReturnUndefined();
    }

    Map *map = node::ObjectWrap::Unwrap<Map>(args.Holder());
    map->get()->setStyleJSON(style, ".");

    NanReturnUndefined();
}

NAN_METHOD(Map::Render) {
    NanScope();

    if (args.Length() != 2) {
        NanThrowError("Wrong number of arguments");
        NanReturnUndefined();
    }

    if (!args[1]->IsFunction()) {
        NanThrowTypeError("Callback must be a function");
        NanReturnUndefined();
    }

    NanCallback *callback = new NanCallback(args[1].As<v8::Function>());

    if (!args[0]->IsObject()) {
        NanThrowTypeError("First argument must be an options object");
        NanReturnUndefined();
    }

    const RenderOptions *options(ParseOptions(args[0]->ToObject()));

    Map *map = node::ObjectWrap::Unwrap<Map>(args.Holder());

    NanAsyncQueueWorker(new RenderWorker(map, options, callback));

    NanReturnUndefined();
}

NAN_METHOD(Map::Terminate) {
    NanScope();

    Map* map = node::ObjectWrap::Unwrap<Map>(args.Holder());
    map->get()->terminate();

    NanReturnUndefined();
}

void Map::Resize(const unsigned int width, const unsigned int height, const float ratio) {
    view_.resize(width, height, ratio);
    map_->resize(width, height, ratio);
}

std::unique_ptr<uint32_t[]> Map::ReadPixels() { return view_.readPixels(); }

} // ns node_mbgl
