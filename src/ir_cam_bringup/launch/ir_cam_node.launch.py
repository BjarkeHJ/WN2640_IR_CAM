from launch import LaunchDescription
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    config = os.path.join(
        get_package_share_directory('ir_cam_bringup'),
        'config',
        'ir_cam_params.yaml'
    )

    node = Node(
        package='usb_cam',
        executable='usb_cam_node_exe',
        name='usb_cam',
        output='screen',
        parameters=[config]
    )

    ld = LaunchDescription([node])

    return ld