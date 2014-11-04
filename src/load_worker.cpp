#include <node_mbgl/load_worker.hpp>

namespace node_mbgl
{

LoadWorker::LoadWorker(Map &map,
                       v8::Handle<v8::Value> &style_handle,
                       v8::Local<v8::Object> &options_obj,
                       const std::string &base_directory,
                       NanCallback *callback)
    : NanAsyncWorker(callback),
      map_(map),
      style_handle_(style_handle),
      options_obj_(options_obj),
      base_directory_(base_directory) {}

LoadWorker::~LoadWorker() {}

void LoadWorker::StringifyStyle() {
    v8::Local<v8::Object> JSON = NanGetCurrentContext()->Global()->Get(NanNew("JSON"))->ToObject();
    v8::Handle<v8::Function> stringify = v8::Handle<v8::Function>::Cast(JSON->Get(NanNew("stringify")));

    style_ = *NanUtf8String(stringify->Call(JSON, 1, &style_handle_));
}

void LoadWorker::ParseOptions() {
    options_ = new LoadOptions();

    options_->zoom = options_obj_->Has(NanNew("zoom")) ? options_obj_->Get(NanNew("zoom"))->NumberValue() : 0;
    options_->bearing = options_obj_->Has(NanNew("bearing")) ? options_obj_->Get(NanNew("bearing"))->NumberValue() : 0;
    options_->latitude = options_obj_->Has(NanNew("center")) ? options_obj_->Get(NanNew("center")).As<v8::Array>()->Get(0)->NumberValue() : 0;
    options_->longitude = options_obj_->Has(NanNew("center")) ? options_obj_->Get(NanNew("center")).As<v8::Array>()->Get(1)->NumberValue() : 0;
    options_->width = options_obj_->Has(NanNew("width")) ? options_obj_->Get(NanNew("width"))->IntegerValue() : 512;
    options_->height = options_obj_->Has(NanNew("height")) ? options_obj_->Get(NanNew("height"))->IntegerValue() : 512;
    options_->pixelRatio = options_obj_->Has(NanNew("pixelRatio")) ? options_obj_->Get(NanNew("pixelRatio"))->IntegerValue() : 1;

    options_->accessToken = *NanUtf8String(options_obj_->Get(NanNew("accessToken")->ToString()));

    if (options_obj_->Has(NanNew("classes"))) {
        v8::Local<v8::Array> classes(options_obj_->Get(NanNew("classes"))->ToObject().As<v8::Array>());
        int length = classes->Length();
        options_->classes.resize(length);
        for (int i = 0; i < length; i++) {
            std::string classname = *NanUtf8String(classes->Get(i)->ToString());
            options_->classes.push_back(classname);
        }
    }
}

void LoadWorker::Execute() {
    StringifyStyle();
    ParseOptions();

    map_.get()->setStyleJSON(style_, base_directory_);
    map_.get()->setAppliedClasses(options_->classes);

    map_.resize(options_->width, options_->height, options_->pixelRatio);

    map_.get()->setLonLatZoom(options_->longitude, options_->latitude, options_->zoom);
    map_.get()->setBearing(options_->bearing);
}

} // ns node_mbgl
