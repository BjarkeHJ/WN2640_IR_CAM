include_directories(SDK/drivers)
include_directories(SDK/interface)

message("Building ARM64")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")

set(CMAKE_C_COMPILER /usr/bin/gcc-7)
set(CMAKE_CXX_COMPILER /usr/bin/g++-7)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/SDK/drivers)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/SDK/interface)

add_executable(
    ir_cmd_uart
    src/ir_camera_cmd.cpp   
)

target_link_libraries(ir_cmd_uart ircam ircmd iruart m)

ament_target_dependencies(ir_cmd_uart rclcpp)
