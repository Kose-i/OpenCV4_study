cmake_minimum_required(VERSION 3.0)
project(Edge_Detection)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Line src/Line.cpp)
target_link_libraries(Line ${OpenCV_LIBS})

add_executable(Line2 src/Line2.cpp)
target_link_libraries(Line2 ${OpenCV_LIBS})

#add_executable(Camera src/Camera.cpp)
#target_link_libraries(Camera ${OpenCV_LIBS})
