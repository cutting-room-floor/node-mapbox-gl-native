#include <node_mbgl/add_worker.hpp>
#include <unistd.h>

namespace node_mbgl
{

AddWorker::AddWorker(Map* map, const double value, NanCallback *callback)
    : NanAsyncWorker(callback),
    map_(map),
    value_(value) {}

AddWorker::~AddWorker() {}

void AddWorker::Execute() {
    sum_ = map_->Val() + value_;
}

void AddWorker::HandleOKCallback() {
    NanScope();

    v8::Local<v8::Value> argv[] = {
        NanNull(),
        NanNew<v8::Number>(sum_)
    };

    callback->Call(2, argv);
};

} // ns node_mbgl
