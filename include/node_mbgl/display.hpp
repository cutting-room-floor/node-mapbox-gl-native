#ifndef NODE_MBGL_DISPLAY
#define NODE_MBGL_DISPLAY

#include <mbgl/platform/default/headless_display.hpp>

namespace node_mbgl
{

extern std::shared_ptr<mbgl::HeadlessDisplay> display_;

} // ns node_mbgl

#endif // NODE_MBGL_DISPLAY
