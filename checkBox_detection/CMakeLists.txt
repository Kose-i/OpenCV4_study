cmake_minimum_required(VERSION 3.0)

project(Questionnair)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(include)

add_executable(main 
  src/align_img.cpp
  src/template_match.cpp
  src/main.cpp
  src/get_checkbox.cpp
)

target_link_libraries(main
  ${OpenCV_LIBS}
#  include
)

add_executable(make_img
  src/a.cpp
  src/get_checkbox.cpp
)
target_link_libraries(make_img
  ${OpenCV_LIBS}
)
