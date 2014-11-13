#ifndef NODE_MBGL_ADDWORKER
#define NODE_MBGL_ADDWORKER

#include <nan.h>
#include <node_mbgl/map.hpp>

namespace node_mbgl
{

class AddWorker : public NanAsyncWorker {
public:
  AddWorker(Map* map, double value, NanCallback *callback);
  ~AddWorker();

  void Execute();
  void HandleOKCallback();

private:
  Map* map_;
  double value_;
  double sum_;
};

} // ns node_mbgl

#endif // NODE_MBGL_ADDWORKER
