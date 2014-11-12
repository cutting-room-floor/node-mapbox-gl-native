#include "addworker.h"
#include <unistd.h>

AddWorker::AddWorker(MyObject* obj, double value, NanCallback *callback)
  : NanAsyncWorker(callback),
  obj_(obj),
  value_(value) {}

AddWorker::~AddWorker() {}

void AddWorker::Execute() {
  sleep(value_);
  sum_ = obj_->Val() + value_;
}

void AddWorker::HandleOKCallback() {
  NanScope();

  v8::Local<v8::Value> argv[] = {
    NanNull(),
    NanNew(sum_)
  };

  callback->Call(2, argv);
};
