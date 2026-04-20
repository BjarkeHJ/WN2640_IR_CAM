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
        'config_ir_params.yaml',
    )

    return LaunchDescription([
        DeclareLaunchArgument('device_1',   default_value='/dev/video2'),
        DeclareLaunchArgument('device_2',   default_value='/dev/video4'),
        DeclareLaunchArgument('encoding', default_value='yuyv'),

        ComposableNodeContainer(
            name='ircam_container_1',
            namespace='',
            package='rclcpp_components',
            executable='component_container_mt',
            output='screen',
            composable_node_descriptions=[
                
                ### Camera 1 ### (Narrow FOV IR Camera)
                ComposableNode(
                    package='ircam_driver',
                    plugin='ircam_stream::IrCameraComponent',
                    name='ircam_narrow',
                    parameters=[{
                        'output_topic':   '/ircam_narrow/raw_image',
                        'device':       LaunchConfiguration('device_1'),
                        'encoding':     LaunchConfiguration('encoding'),
                        'width':        640,
                        'height':       512,
                        'fps':          30,
                        'v4l2_buffers': 8,
                        'frame_id':     'ir_camera_narrow',
                        'queue_depth':  2,
                    }],
                    extra_arguments=[
                        {'use_intra_process_comms': True},
                    ],
                ),

                ComposableNode(
                    package='ircam_driver',
                    plugin='ircam_stream::IrCameraH264Component',
                    name='ircam_h264_narrow',
                    parameters=[{
                        'input_topic':    '/ircam_narrow/raw_image',
                        'output_topic':   '/ircam_narrow/h264',
                        'preset':         'ultrafast',
                        'crf':            23,
                    }],
                    extra_arguments=[
                        {'use_intra_process_comms': True},
                    ],
                ),
            ],
        ),
        ComposableNodeContainer(
            name='ircam_container_2',
            namespace='',
            package='rclcpp_components',
            executable='component_container_mt',
            output='screen',
            composable_node_descriptions=[
                ### Camera 2 ### (Wide FOV IR Camera)
                ComposableNode(
                    package='ircam_driver',
                    plugin='ircam_stream::IrCameraComponent',
                    name='ircam_wide',
                    parameters=[{
                        'output_topic':   '/ircam_wide/raw_image',
                        'device':       LaunchConfiguration('device_2'),
                        'encoding':     LaunchConfiguration('encoding'),
                        'width':        640,
                        'height':       512,
                        'fps':          30,
                        'v4l2_buffers': 8,
                        'frame_id':     'ir_camera_wide',
                        'queue_depth':  2,
                    }],
                    extra_arguments=[
                        {'use_intra_process_comms': True},
                    ],
                ),

                ComposableNode(
                    package='ircam_driver',
                    plugin='ircam_stream::IrCameraH264Component',
                    name='ircam_h264_wide',
                    parameters=[{
                        'input_topic':    '/ircam_wide/raw_image',
                        'output_topic':   '/ircam_wide/h264',
                        'preset':         'ultrafast',
                        'crf':            23,
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
        TimerAction(
            period=30.0, # seconds delay for bringup code to run
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
