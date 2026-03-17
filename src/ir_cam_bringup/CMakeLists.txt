cmake_minimum_required(VERSION 3.8)
project(ir_cam_bringup)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ROS usb_cam bringup
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

install(
  DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)

# UART SDK Control
add_subdirectory(./SDK_control/)

# set_target_properties(
#   ir_cmd_uart PROPERTIES
#   INSTALL_RPATH "$ORIGIN/../../lib/${PROJECT_NAME}"
#   INSTALL_RPATH_USE_LINK_PATH TRUE
# )
# install(
#   TARGETS ir_cmd_uart
#   RUNTIME DESTINATION lib/${PROJECT_NAME}
# )
# install(
#   DIRECTORY ./SDK_control/SDK/drivers/
#   DESTINATION lib/${PROJECT_NAME}
#   FILES_MATCHING PATTERN "*.so*"
# )

ament_package()
