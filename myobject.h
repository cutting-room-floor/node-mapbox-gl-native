#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <nan.h>

class MyObject : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);
  static NAN_METHOD(Set);
  static NAN_METHOD(Add);
  double Val() const { return val_; }

 private:
  MyObject();
  ~MyObject();

  static v8::Persistent<v8::Function> constructor;
  static NAN_METHOD(New);
  static NAN_METHOD(NewInstance);
  double val_;
};

#endif
