#include "node_view.hpp"

#include <mbgl/platform/default/headless_display.hpp>
#include <mbgl/map/still_image.hpp>
#include <mbgl/util/exception.hpp>

#include <unistd.h>

namespace node_mbgl {

struct NodeView::Options {
    uint16_t width = 512;
    uint16_t height = 512;
    float ratio = 1.0f;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// Static Node Methods

v8::Persistent<v8::FunctionTemplate> NodeView::constructorTemplate;

static std::shared_ptr<mbgl::HeadlessDisplay> sharedDisplay() {
    static auto display = std::make_shared<mbgl::HeadlessDisplay>();
    return display;
}

void NodeView::Init(v8::Handle<v8::Object> target) {
    NanScope();

    v8::Local<v8::FunctionTemplate> t = NanNew<v8::FunctionTemplate>(New);

    t->InstanceTemplate()->SetInternalFieldCount(1);
    t->SetClassName(NanNew("View"));

    NanAssignPersistent(constructorTemplate, t);

    target->Set(NanNew("View"), t->GetFunction());

    // Initialize display connection on module load.
    sharedDisplay();
}

NAN_METHOD(NodeView::New) {
    NanScope();

    if (!args.IsConstructCall()) {
        return NanThrowTypeError("Use the new operator to create new View objects");
    }

    if (args.Length() < 1 || !args[0]->IsObject()) {
        return NanThrowTypeError("Requires an options object as first argument");
    }

    auto obj = args[0]->ToObject();
    auto options = new Options;

    if (obj->Has(NanNew("width")) && obj->Get(NanNew("width"))->IsNumber()) {
        options->width = obj->Get(NanNew("width"))->IntegerValue();
    } else {
        return NanThrowError("Options object must have a numerical width attribute");
    }

    if (obj->Has(NanNew("height")) && obj->Get(NanNew("height"))->IsNumber()) {
        options->height = obj->Get(NanNew("height"))->IntegerValue();
    } else {
        return NanThrowError("Options object must have a numerical height attribute");
    }

    if (obj->Has(NanNew("ratio")) && obj->Get(NanNew("ratio"))->IsNumber()) {
        options->ratio = obj->Get(NanNew("ratio"))->IntegerValue();
    } else {
        return NanThrowError("Options object must have a numerical ratio attribute");
    }

    try {
        auto nodeView = new NodeView(options);
        nodeView->Wrap(args.This());
    } catch(std::exception &ex) {
        return NanThrowError(ex.what());
    }

    NanReturnValue(args.This());
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Instance

NodeView::NodeView(Options* options)
  : view(std::make_unique<mbgl::HeadlessView>(sharedDisplay(), options->width, options->height, options->ratio)) {}

NodeView::~NodeView() {
    view.reset();
}

}
