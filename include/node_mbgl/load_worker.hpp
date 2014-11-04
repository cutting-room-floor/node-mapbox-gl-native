#ifndef NODE_MBGL_LOAD_WORKER
#define NODE_MBGL_LOAD_WORKER

#include <node.h>
#include <nan.h>

#include <node_mbgl/map.hpp>

#include <string>
#include <vector>

namespace node_mbgl
{

struct LoadOptions {
    double zoom;
    double bearing;
    double latitude;
    double longitude;
    unsigned int width;
    unsigned int height;
    unsigned int pixelRatio;
    std::string accessToken;
    std::vector<std::string> classes;
};

class LoadWorker : public NanAsyncWorker {
public:
    LoadWorker(Map &map,
               v8::Handle<v8::Value> &style_handle,
               v8::Local<v8::Object> &options_obj,
               const std::string &base_directory,
               NanCallback *callback);
    ~LoadWorker();

    void StringifyStyle();
    void ParseOptions();

    void Execute();

private:
    Map map_;
    v8::Handle<v8::Value> style_handle_;
    v8::Local<v8::Object> options_obj_;
    const std::string base_directory_;
    NanCallback callback_;

    std::string style_;
    LoadOptions *options_;
};

} // ns node_mbgl

#endif // NODE_MBGL_LOAD_WORKER
