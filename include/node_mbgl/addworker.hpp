#ifndef NODE_MBGL_ADDWORKER
#define NODE_MBGL_ADDWORKER

#include <nan.h>
#include <node_mbgl/myobject.hpp>

namespace node_mbgl
{

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

} // ns node_mbgl

#endif // NODE_MBGL_ADDWORKER
