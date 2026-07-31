from launch import LaunchDescription
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    config = os.path.join(
        get_package_share_directory('ircam_driver'),
        'config',
        'config_ir_params.yaml',
    )

    node = Node(
        package='ircam_driver',
        executable='ircam_cmd_uart',
        name='ircam_cmd_uart',
        output='screen',
        parameters=[config]
    )
    
    ld = LaunchDescription([node])

    return ld