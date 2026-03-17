set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")

set(IR_COMP "ircam_component")
SET(IR_ENC_COMP "ircam_h264_component")

add_library(${IR_COMP} SHARED
    src/v4l2_capture.cpp
    src/ircam_node.cpp
    src/ircam_component.cpp
)

