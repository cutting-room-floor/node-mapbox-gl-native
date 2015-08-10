#pragma once

#include <mbgl/storage/file_source.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wshadow"
#include <node.h>
#include <node_version.h>
#include <nan.h>
#pragma GCC diagnostic pop

#include <unordered_map>

namespace node_mbgl {

namespace util { template <typename T> class AsyncQueue; }

class NodeFileSource : public mbgl::FileSource {
public:
    NodeFileSource(v8::Handle<v8::Object>);
    ~NodeFileSource();

    mbgl::Request* request(const mbgl::Resource&, uv_loop_t*, Callback);
    void cancel(mbgl::Request*);

    // visiblity?
    void notify(mbgl::Request*, const std::shared_ptr<const mbgl::Response>&);

private:
    struct Action;
    using Queue = util::AsyncQueue<Action>;

    void processAdd(mbgl::Request*);
    void processCancel(mbgl::Request*);

    v8::Persistent<v8::Object> options;

private:
#if (NODE_MODULE_VERSION > NODE_0_10_MODULE_VERSION)
    std::unordered_map<mbgl::Request*, v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>>> pending;
#else
    std::unordered_map<mbgl::Request*, v8::Persistent<v8::Object>> pending;
#endif
    Queue *queue = nullptr;
};

}
