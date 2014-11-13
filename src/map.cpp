#include <node.h>
#include <node_mbgl/map.hpp>
#include <node_mbgl/display.hpp>
#include <node_mbgl/addworker.hpp>

namespace node_mbgl
{

Map::Map()
    : view_(display_),
    map_(std::make_shared<mbgl::Map>(view_)) {};

Map::~Map() {};

v8::Persistent<v8::Function> Map::constructor;

void Map::Init(v8::Handle<v8::Object> exports) {
    NanScope();

    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl =
        NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew("Map"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "load", Load);
    NODE_SET_PROTOTYPE_METHOD(tpl, "set", Set);
    NODE_SET_PROTOTYPE_METHOD(tpl, "add", Add);

    NanAssignPersistent(constructor, tpl->GetFunction());

    exports->Set(NanNew("Map"),
        NanNew<v8::FunctionTemplate>(NewInstance)->GetFunction());
}

NAN_METHOD(Map::New) {
    NanScope();

    Map* map = new Map();
    map->Wrap(args.Holder());

    NanReturnHolder();
}

NAN_METHOD(Map::NewInstance) {
    NanScope();

    if (!args.IsConstructCall())
    {
        NanThrowError("Cannot call constructor as function, you need to use 'new' keyword");
        NanReturnUndefined();
    }

    const unsigned argc = 0;
    v8::Local<v8::Value> argv[argc] = {};
    v8::Local<v8::Function> cons = NanNew<v8::Function>(constructor);
    v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

    NanReturnValue(instance);
}

const std::string Map::StringifyStyle(v8::Handle<v8::Value> style_handle) {
    NanScope();

    v8::Handle<v8::Object> JSON = NanGetCurrentContext()->Global()->Get(NanNew("JSON"))->ToObject();
    v8::Handle<v8::Function> stringify = v8::Handle<v8::Function>::Cast(JSON->Get(NanNew("stringify")));

    return *NanUtf8String(stringify->Call(JSON, 1, &style_handle));
}

const LoadOptions* Map::ParseOptions(v8::Local<v8::Object> obj) {
    NanScope();

    LoadOptions* options = new LoadOptions();

    options->zoom = obj->Has(NanNew("zoom")) ? obj->Get(NanNew("zoom"))->NumberValue() : 0;
    options->bearing = obj->Has(NanNew("bearing")) ? obj->Get(NanNew("bearing"))->NumberValue() : 0;
    options->latitude = obj->Has(NanNew("center")) ? obj->Get(NanNew("center")).As<v8::Array>()->Get(0)->NumberValue() : 0;
    options->longitude = obj->Has(NanNew("center")) ? obj->Get(NanNew("center")).As<v8::Array>()->Get(1)->NumberValue() : 0;
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

    if (args.Length() != 1)
    {
        NanThrowTypeError("Wrong number of arguments");
        NanReturnUndefined();
    }

    if (!args[0]->IsObject())
    {
        NanThrowTypeError("First argument must be a style object");
        NanReturnUndefined();
    }
    
    const std::string style(StringifyStyle(args[0]));
    
    /*
    if (!args[1]->IsObject())
    {
        NanThrowTypeError("Second argument must be an options object");
        NanReturnUndefined();
    }

    const LoadOptions* options(ParseOptions(args[1]->ToObject()));
    */

    Map* map = node::ObjectWrap::Unwrap<Map>(args.Holder());

    const std::string base_directory("/");

    map->get()->setStyleJSON(style, base_directory);

    /*
    map->get()->setAppliedClasses(options->classes);

    map->Resize(options->width, options->height, options->ratio);

    map->get()->setLonLatZoom(options->longitude, options->latitude, options->zoom);
    map->get()->setBearing(options->bearing);
    */

    NanReturnUndefined();
}

NAN_METHOD(Map::Set) {
    NanScope();

    Map* map = node::ObjectWrap::Unwrap<Map>(args.Holder());
    map->val_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();

    NanReturnHolder();
}

NAN_METHOD(Map::Add) {
    NanScope();

    Map* map = node::ObjectWrap::Unwrap<Map>(args.Holder());
    double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    NanCallback *callback = new NanCallback(args[1].As<v8::Function>());

    NanAsyncQueueWorker(new AddWorker(map, value, callback));

    NanReturnUndefined();
}

} // ns node_mbgl
