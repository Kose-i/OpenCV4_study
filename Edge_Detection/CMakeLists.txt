cmake_minimum_required(VERSION 3.0)
project(Edge_Detection)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Laplacian src/Laplacian.cpp)
target_link_libraries(Laplacian ${OpenCV_LIBS})

add_executable(LoG src/LoG.cpp)
target_link_libraries(LoG ${OpenCV_LIBS})
