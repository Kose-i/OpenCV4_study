cmake_minimum_required(VERSION 3.0)
project(Edge_Detection)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Laplacian src/Laplacian.cpp)
target_link_libraries(Laplacian ${OpenCV_LIBS})

add_executable(Sobel src/Sobel.cpp)
target_link_libraries(Sobel ${OpenCV_LIBS})

add_executable(LoG src/LoG.cpp)
target_link_libraries(LoG ${OpenCV_LIBS})

add_executable(DoG src/DoG.cpp)
target_link_libraries(DoG ${OpenCV_LIBS})

add_executable(LineOperate src/LineOperate.cpp)
target_link_libraries(LineOperate ${OpenCV_LIBS})

add_executable(Canny src/Canny.cpp)
target_link_libraries(Canny ${OpenCV_LIBS})

add_executable(Canny-Color src/Canny_color.cpp)
target_link_libraries(Canny-Color ${OpenCV_LIBS})

add_executable(LineDetection src/LineDetection.cpp)
target_link_libraries(LineDetection ${OpenCV_LIBS})
