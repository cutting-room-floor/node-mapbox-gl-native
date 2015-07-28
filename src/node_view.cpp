#include "node_view.hpp"

#include <mbgl/platform/default/headless_display.hpp>
#include <mbgl/map/still_image.hpp>
#include <mbgl/util/exception.hpp>

#include <unistd.h>

namespace node_mbgl {

struct NodeView::Options {
    float ratio = 1.0f;
    uint16_t width = 512;
    uint16_t height = 512;
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

    if (obj->Has(NanNew("ratio"))) { options->ratio = obj->Get(NanNew("ratio"))->IntegerValue(); }
    if (obj->Has(NanNew("width"))) { options->width = obj->Get(NanNew("width"))->IntegerValue(); }
    if (obj->Has(NanNew("height"))) { options->height = obj->Get(NanNew("height"))->IntegerValue(); }

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
  : view(sharedDisplay(), options->ratio, options->width, options->height) {}

NodeView::~NodeView() {
    // view.reset(nullptr);
}

}
