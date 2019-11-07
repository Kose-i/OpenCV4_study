cmake_minimum_required(VERSION 3.0)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Test src/test.cpp)

add_executable(BasicTransform src/BasicTransform.cpp)
target_link_libraries(BasicTransform ${OpenCV_LIBS})
