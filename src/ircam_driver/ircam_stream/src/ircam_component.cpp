#include "ircam_node.hpp"
#include <rclcpp_components/register_node_macro.hpp>

namespace ircam_stream {

// thin alias for the node. Forward the nodeoptions
class IrCameraComponent : public IrCameraNode {
public:
    explicit IrCameraComponent(const rclcpp::NodeOptions& options) : IrCameraNode(options) {}
};

} // namespace ircam_stream

RCLCPP_COMPONENTS_REGISTER_NODE(ircam_stream::IrCameraComponent)