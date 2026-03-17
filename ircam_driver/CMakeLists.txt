cmake_minimum_required(VERSION 3.8)
project(ircam_driver)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(sensor_msgs REQUIRED)

# IRCAM CONTROL: UART SDK Control
add_subdirectory(./ircam_control/)

# IRCAM STREAM: V4L2 -> ROS2 Driver + H264 Encoding
add_subdirectory(./ircam_stream/)

# Install Launch and Config
install(
  DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
