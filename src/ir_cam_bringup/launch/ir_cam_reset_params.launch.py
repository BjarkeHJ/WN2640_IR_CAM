from launch import LaunchDescription
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    config = os.path.join(
        get_package_share_directory('ir_cam_bringup'),
        'config',
        'reset_to_default_params.yaml'
    )

    node = Node(
        package='ir_cam_bringup',
        executable='ir_cmd_uart',
        name='ir_cmd_uart',
        output='screen',
        parameters=[config]
    )
    
    ld = LaunchDescription([node])

    return ld