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
  src/make_template_mask.cpp
  src/get_checkbox.cpp
)
target_link_libraries(make_img
  ${OpenCV_LIBS}
)

add_executable(make_histogram
  src/get_histogram.cpp
)
target_link_libraries(make_histogram
  ${OpenCV_LIBS}
)

add_executable(test_align
  src/align_test.cpp
  src/align_img.cpp
)
target_link_libraries(test_align
  ${OpenCV_LIBS}
)
