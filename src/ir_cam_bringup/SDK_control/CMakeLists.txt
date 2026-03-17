set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")

add_executable(
  ir_cmd_uart
  src/ir_camera_cmd.cpp   
)

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
  message("Building x86_64")
  set(SDK_DRIVER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDK/drivers/x86_64)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
  message("Building aarch64")
  set(SDK_DRIVER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDK/drivers/aarch64)
  target_compile_definitions(ir_cmd_uart PRIVATE USE_ONBOARD_UART)
endif()

target_include_directories(ir_cmd_uart PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/SDK/interface
  ${SDK_DRIVER_DIR}
)

target_link_libraries(ir_cmd_uart
  ${SDK_DRIVER_DIR}/libircam.so
  ${SDK_DRIVER_DIR}/libircmd.so
  ${SDK_DRIVER_DIR}/libiruart.so
  m
)

ament_target_dependencies(ir_cmd_uart rclcpp)

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
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/SDK/drivers/${CMAKE_SYSTEM_PROCESSOR}/
  DESTINATION lib/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.so*"
)
