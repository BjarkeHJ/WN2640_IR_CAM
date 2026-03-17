set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")

set(IR_COMP "ircam_component")
set(IR_ENC_COMP "ircam_h264_component")

find_package(PkgConfig REQUIRED)

pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
  libavcodec
  libavutil
  libswscale)

pkg_check_modules(AVCODEC  REQUIRED IMPORTED_TARGET libavcodec)
pkg_check_modules(AVUTIL   REQUIRED IMPORTED_TARGET libavutil)
pkg_check_modules(SWSCALE  REQUIRED IMPORTED_TARGET libswscale)

# IRCAM RAW STREAM COMPONENT
add_library(${IR_COMP} SHARED
    src/v4l2_capture.cpp
    src/ircam_node.cpp
    src/ircam_component.cpp
)

target_include_directories(${IR_COMP} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(${IR_COMP}
    rclcpp
    rclcpp_components
    sensor_msgs
)

install(
  TARGETS ${IR_COMP}
  EXPORT export_ircam_driver
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)


# IRCAM ENCODED STREAM COMPONENT
add_library(${IR_ENC_COMP} SHARED
    src/ircam_h264_node.cpp
    src/ircam_h264_component.cpp
)

target_include_directories(${IR_ENC_COMP} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>  
)

ament_target_dependencies(${IR_ENC_COMP}
  rclcpp
  rclcpp_components
  sensor_msgs
)

target_link_libraries(${IR_ENC_COMP}
  PkgConfig::AVCODEC
  PkgConfig::AVUTIL
  PkgConfig::SWSCALE
)

install(
  TARGETS ${IR_ENC_COMP}
  EXPORT export_ircam_driver
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  DIRECTORY include/
  DESTINATION include
)