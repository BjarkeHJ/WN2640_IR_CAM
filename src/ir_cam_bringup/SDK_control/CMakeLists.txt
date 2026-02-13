# PROJECT(ir_cmd_uart)
# CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

# set(PLATFORM_NAME x64)
# set(CMAKE_SYSTEM_NAME Linux)
# set(CMAKE_C_COMPILER gcc)
# set(CMAKE_C_FLAGS -I/include)
# set(CMAKE_CXX_COMPILER g++)
# set(CMAKE_FIND_ROOT_PATH )
# set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# list(APPEND CMAKE_PREFIX_PATH )

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
