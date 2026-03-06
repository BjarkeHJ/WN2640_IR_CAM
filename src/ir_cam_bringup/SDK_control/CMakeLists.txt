include_directories(SDK/drivers)
include_directories(SDK/interface)

message("Building x64")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/SDK/drivers)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/SDK/interface)

add_executable(
    ir_cmd_uart
    src/ir_camera_cmd.cpp   
)

target_link_libraries(ir_cmd_uart ircam ircmd iruart m)
