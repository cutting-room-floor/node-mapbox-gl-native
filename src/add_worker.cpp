#include <node_mbgl/add_worker.hpp>
#include <unistd.h>

#include <iostream>

namespace node_mbgl
{

AddWorker::AddWorker(Map* map, const double value, NanCallback *callback)
  : NanAsyncWorker(callback),
  map_(map),
  value_(value) {}

AddWorker::~AddWorker() {}

void AddWorker::Execute() {
  std::cout << "map_->Val(): " << map_->Val() << '\n';
  std::cout << "value_: " << value_ << '\n';
  sleep(value_);
  sum_ = map_->Val() + value_;
  std::cout << "sum_: " << sum_ << '\n';
}

void AddWorker::HandleOKCallback() {
  NanScope();

  v8::Local<v8::Value> argv[] = {
    NanNull(),
    NanNew<v8::Number>(sum_)
  };

  std::cout << "callback: " << sum_ << '\n';

  callback->Call(2, argv);
};

} // ns node_mbgl
