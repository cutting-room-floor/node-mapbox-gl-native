#include <node_mbgl/render.hpp>
#include <node_mbgl/render_worker.hpp>

namespace node_mbgl
{

NAN_METHOD(Render) {
    NanScope();

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
    options->latitude = v8options->Has(NanNew("latitude")) ? v8options->Get(NanNew("latitude"))->NumberValue() : 0;
    options->longitude = v8options->Has(NanNew("longitude")) ? v8options->Get(NanNew("longitude"))->NumberValue() : 0;
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

    const std::string style(*v8::String::Utf8Value(args[0].As<v8::String>()));
    const std::string base_directory(*v8::String::Utf8Value(args[2].As<v8::String>()));
    NanCallback *callback = new NanCallback(args[3].As<v8::Function>());

    NanAsyncQueueWorker(new RenderWorker(style,
                                         options,
                                         base_directory,
                                         callback));

    NanReturnUndefined();
}

NAN_METHOD(RenderSync) {
    NanScope();

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
    options->latitude = v8options->Has(NanNew("latitude")) ? v8options->Get(NanNew("latitude"))->NumberValue() : 0;
    options->longitude = v8options->Has(NanNew("longitude")) ? v8options->Get(NanNew("longitude"))->NumberValue() : 0;
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

    const std::string style(*v8::String::Utf8Value(args[0].As<v8::String>()));
    const std::string base_directory(*v8::String::Utf8Value(args[2].As<v8::String>()));

    RenderWorker worker(style,
                        options,
                        base_directory,
                        nullptr);

    const std::string image = worker.Render();

    NanReturnValue(NanNewBufferHandle(image.c_str(), image.length()));
}

} // ns node_mbgl
