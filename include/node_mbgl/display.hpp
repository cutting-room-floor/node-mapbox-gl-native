#ifndef NODE_MBGL_DISPLAY
#define NODE_MBGL_DISPLAY

#include "../../vendor/mapbox-gl-native/common/headless_display.hpp"

namespace node_mbgl
{

extern std::shared_ptr<mbgl::HeadlessDisplay> display_;

} // ns node_mbgl

#endif // NODE_MBGL_DISPLAY
