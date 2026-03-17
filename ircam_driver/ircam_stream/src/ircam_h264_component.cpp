#include "ircam_h264_node.hpp"
#include <rclcpp_components/register_node_macro.hpp>

namespace ircam_stream {

// thin alias for the node. Forward the nodeoptions
class IrCameraH264Component : public IrcamH264Republisher {
public:
    explicit IrCameraH264Component(const rclcpp::NodeOptions& options) : IrcamH264Republisher(options) {}
};

} // namespace ircam_stream

RCLCPP_COMPONENTS_REGISTER_NODE(ircam_stream::IrCameraH264Component)