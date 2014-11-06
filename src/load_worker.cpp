#include <node_mbgl/load_worker.hpp>

namespace node_mbgl
{

LoadWorker::LoadWorker(Map *map,
                       const std::string &style,
                       const LoadOptions *options,
                       const std::string &base_directory,
                       NanCallback *callback)
    : NanAsyncWorker(callback),
      map_(map),
      style_(style),
      options_(options),
      base_directory_(base_directory) {}

LoadWorker::~LoadWorker() {}

void LoadWorker::Execute() {
    map_->get()->setStyleJSON(style_, base_directory_);
    map_->get()->setAppliedClasses(options_->classes);

    map_->Resize(options_->width, options_->height, options_->ratio);

    map_->get()->setLonLatZoom(options_->longitude, options_->latitude, options_->zoom);
    map_->get()->setBearing(options_->bearing);
}

void LoadWorker::HandleOKCallback() {
    NanScope();

    v8::Local<v8::Value> argv[] = {
        NanNull()
    };

    callback->Call(1, argv);

    map_->_unref();
};

} // ns node_mbgl
