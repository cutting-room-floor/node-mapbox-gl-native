#ifndef ADDWORKER_H
#define ADDWORKER_H

#include <nan.h>
#include "myobject.h"

class AddWorker : public NanAsyncWorker {
public:
  AddWorker(MyObject* obj, double value, NanCallback *callback);
  ~AddWorker();

  void Execute();
  void HandleOKCallback();

private:
  MyObject* obj_;
  double value_;
  double sum_;
};

#endif
