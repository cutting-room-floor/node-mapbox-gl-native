#ifndef NODE_MBGL_LOAD_WORKER
#define NODE_MBGL_LOAD_WORKER

#include <node.h>
#include <nan.h>

#include <node_mbgl/map.hpp>

#include <string>
#include <vector>

namespace node_mbgl
{

class LoadWorker : public NanAsyncWorker {
public:
    LoadWorker(Map *map,
               const std::string &style,
               const LoadOptions *options,
               const std::string &base_directory,
               NanCallback *callback);
    ~LoadWorker();

    void Execute();
    void HandleOKCallback();

private:
    Map *map_;
    const std::string style_;
    const LoadOptions *options_;
    const std::string base_directory_;
    NanCallback callback_;
};

} // ns node_mbgl

#endif // NODE_MBGL_LOAD_WORKER
