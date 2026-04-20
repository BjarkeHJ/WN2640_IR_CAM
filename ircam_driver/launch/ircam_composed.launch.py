import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, TimerAction
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import ComposableNodeContainer, Node
from launch_ros.descriptions import ComposableNode

def generate_launch_description():
    #     pkg_share = get_package_share_directory('aerial_tn')
    #     default_params = os.path.join(pkg_share, 'config', 'ircam_params.yaml')

    cmd_config = os.path.join(
        get_package_share_directory('ircam_driver'),
        'config',
        'config_ir_params.yaml'
    )

    return LaunchDescription([
        DeclareLaunchArgument('device',   default_value='/dev/video2'),
        DeclareLaunchArgument('encoding', default_value='yuyv'),

        ComposableNodeContainer(
            name='ircam_container',
            namespace='',
            package='rclcpp_components',
            executable='component_container_mt',
            output='screen',
            composable_node_descriptions=[

                ComposableNode(
                    package='ircam_driver',
                    plugin='ircam_stream::IrCameraComponent',
                    name='ircam',
                    parameters=[{
                        'device':       LaunchConfiguration('device'),
                        'encoding':     LaunchConfiguration('encoding'),
                        'width':        640,
                        'height':       512,
                        'fps':          30,
                        'v4l2_buffers': 8,
                        'frame_id':     'ir_camera',
                        'queue_depth':  2,
                    }],
                    extra_arguments=[
                        {'use_intra_process_comms': True},
                    ],
                ),

                ComposableNode(
                    package='ircam_driver',
                    plugin='ircam_stream::IrCameraH264Component',
                    name='ircam_h264',
                    parameters=[{
                        'input_topic':    '/ircam/raw_image',
                        'output_topic':   '/ircam/h264',
                        'preset':         'ultrafast',
                        'crf':            23, # constraint rate factor for quality control (lower is better quality, 23 is default)
                    }],
                    extra_arguments=[
                        {'use_intra_process_comms': True},
                    ],
                ),
            ],
        ),

        TimerAction(
            period=5.0, # seconds delay for bringup code to run
            actions=[
                Node(
                    package='ircam_driver',
                    executable='ircam_cmd_uart',
                    name='ircam_cmd_uart',
                    output='screen',
                    parameters=[cmd_config]
                ),
            ]
        ),
    ])
