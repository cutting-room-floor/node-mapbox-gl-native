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

    NanReturnValue(args.Holder());
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

NAN_METHOD(Map::Set) {
    NanScope();

    Map* map = node::ObjectWrap::Unwrap<Map>(args.Holder());
    map->val_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();

    NanReturnValue(args.Holder());
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
