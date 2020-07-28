cmake_minimum_required(VERSION 3.0)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(include)

add_executable(main 
  src/main.cpp
)

target_link_libraries(main
  ${OpenCV_LIBS}
)
