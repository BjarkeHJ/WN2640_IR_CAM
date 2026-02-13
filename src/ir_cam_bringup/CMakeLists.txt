cmake_minimum_required(VERSION 3.8)
project(ir_cam_bringup)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ROS usb_cam bringup
find_package(ament_cmake REQUIRED)
install(
  DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME}
)

# UART SDK Control
add_subdirectory(./SDK_control/)
set_target_properties(
  ir_cmd_uart PROPERTIES
  INSTALL_RPATH "$ORIGIN/../../lib/${PROJECT_NAME}"
  INSTALL_RPATH_USE_LINK_PATH TRUE
)
install(
  TARGETS ir_cmd_uart
  RUNTIME DESTINATION lib/${PROJECT_NAME}
)
install(
  DIRECTORY ./SDK_control/SDK/drivers/
  DESTINATION lib/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.so*"
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
