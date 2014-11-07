#include <node_mbgl/render.hpp>
#include <node_mbgl/render_worker.hpp>

namespace node_mbgl
{

NAN_METHOD(Render) {
    NanScope();

    if (args.Length() != 4)
    {
        NanThrowTypeError("Too few arguments");
        NanReturnUndefined();
    }

    if (!args[0]->IsObject())
    {
        NanThrowTypeError("first argument must be a style object");
        NanReturnUndefined();
    }
    
    v8::Local<v8::Object> JSON = NanGetCurrentContext()->Global()->Get(NanNew("JSON"))->ToObject();
    v8::Handle<v8::Function> stringify = v8::Handle<v8::Function>::Cast(JSON->Get(NanNew("stringify")));

    v8::Handle<v8::Value> style_handle = args[0];
    const std::string style(*v8::String::Utf8Value(stringify->Call(JSON, 1, &style_handle)));

    if (!args[1]->IsObject())
    {
        NanThrowTypeError("second argument must be an options object");
        NanReturnUndefined();
    }

    v8::Local<v8::Object> v8options = NanNew<v8::Object>();
    v8options = args[1]->ToObject();
    renderWorkerOptions *options = new renderWorkerOptions();

    options->zoom = v8options->Has(NanNew("zoom")) ? v8options->Get(NanNew("zoom"))->NumberValue() : 0;
    options->bearing = v8options->Has(NanNew("bearing")) ? v8options->Get(NanNew("bearing"))->NumberValue() : 0;
    options->latitude = v8options->Has(NanNew("center")) ? v8options->Get(NanNew("center")).As<v8::Array>()->Get(0)->NumberValue() : 0;
    options->longitude = v8options->Has(NanNew("center")) ? v8options->Get(NanNew("center")).As<v8::Array>()->Get(1)->NumberValue() : 0;
    options->width = v8options->Has(NanNew("width")) ? v8options->Get(NanNew("width"))->IntegerValue() : 512;
    options->height = v8options->Has(NanNew("height")) ? v8options->Get(NanNew("height"))->IntegerValue() : 512;
    options->pixelRatio = v8options->Has(NanNew("pixelRatio")) ? v8options->Get(NanNew("pixelRatio"))->IntegerValue() : 1;

    v8::String::Utf8Value accessToken(v8options->Get(NanNew("accessToken"))->ToString());
    options->accessToken = std::string(*accessToken);

    if (v8options->Has(NanNew("classes"))) {
        v8::Local<v8::Array> classes(v8options->Get(NanNew("classes"))->ToObject().As<v8::Array>());
        int length = classes->Length();
        options->classes.resize(length);
        for (int i = 0; i < length; i++) {
            std::string classname = *v8::String::Utf8Value(classes->Get(i)->ToString());
            options->classes.push_back(classname);
        }
    }

    if (!args[2]->IsString())
    {
        NanThrowTypeError("third argument must be a string");
        NanReturnUndefined();
    }


    if (!args[3]->IsFunction())
    {
        NanThrowTypeError("fourth argument must be a callback");
        NanReturnUndefined();
    }

    const std::string base_directory(*v8::String::Utf8Value(args[2].As<v8::String>()));
    NanCallback *callback = new NanCallback(args[3].As<v8::Function>());

    NanAsyncQueueWorker(new RenderWorker(style,
                                         options,
                                         base_directory,
                                         callback));

    NanReturnUndefined();
}

} // ns node_mbgl
