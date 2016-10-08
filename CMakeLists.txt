cmake_minimum_required(VERSION 3.5)
project(calibration)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp
        MYfunctions.cpp MYfunctions.h
        Camera_calibrator.cpp Camera_calibrator.h Smart_point2f.cpp Smart_point2f.h My_exeptions.cpp My_exeptions.h Video_controller.cpp Video_controller.h)

find_package( OpenCV 3 REQUIRED )


add_executable(calibration ${SOURCE_FILES})

target_link_libraries( calibration ${OpenCV_LIBS} )
