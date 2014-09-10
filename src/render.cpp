#include <node_mbgl/render.hpp>
#include <node_mbgl/render_worker.hpp>

namespace node_mbgl
{

NAN_METHOD(Render) {
    NanScope();

    const std::string style(*v8::String::Utf8Value(args[0].As<v8::String>()));
    const std::string info(*v8::String::Utf8Value(args[1].As<v8::String>()));
    const std::string base_directory(*v8::String::Utf8Value(args[2].As<v8::String>()));
    NanCallback *callback = new NanCallback(args[3].As<v8::Function>());

    NanAsyncQueueWorker(new RenderWorker(style,
                                         info,
                                         base_directory,
                                         callback));

    NanReturnUndefined();
}

NAN_METHOD(RenderSync) {
    NanScope();

    const std::string style(*v8::String::Utf8Value(args[0].As<v8::String>()));
    const std::string info(*v8::String::Utf8Value(args[1].As<v8::String>()));
    const std::string base_directory(*v8::String::Utf8Value(args[2].As<v8::String>()));

    RenderWorker worker(style,
                        info,
                        base_directory,
                        nullptr);

    const std::string image = worker.Render();

    NanReturnValue(NanNewBufferHandle(image.c_str(), image.length()));
}

} // ns node_mbgl
