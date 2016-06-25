cmake_minimum_required(VERSION 3.5)
project(calibration)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp
        MYfunctions.cpp MYfunctions.h
        )

find_package( OpenCV REQUIRED )


add_executable(calibration ${SOURCE_FILES})

target_link_libraries( calibration ${OpenCV_LIBS} )
