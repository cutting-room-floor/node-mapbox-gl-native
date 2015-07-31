#pragma once

#include <mbgl/platform/default/headless_view.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wshadow"
#include <node.h>
#include <nan.h>
#pragma GCC diagnostic pop

namespace node_mbgl {

class NodeView : public node::ObjectWrap {

struct Options;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Static Node Methods
public:
    static void Init(v8::Handle<v8::Object> target);
    static NAN_METHOD(New);

    static v8::Persistent<v8::FunctionTemplate> constructorTemplate;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Instance
private:
    NodeView(Options*);
    ~NodeView();

    mbgl::HeadlessView view;

public:
    inline mbgl::HeadlessView* get() {
        return &view;
    }

};

}
