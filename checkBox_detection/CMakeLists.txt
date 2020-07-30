cmake_minimum_required(VERSION 3.0)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(include)

add_executable(main 
  src/align_img.cpp
  src/template_match.cpp
  src/main.cpp
)

target_link_libraries(main
  ${OpenCV_LIBS}
#  include
)

add_executable(rectangle_detect
  src/rectangle_detection.cpp
)

target_link_libraries(rectangle_detect
  ${OpenCV_LIBS}
)
