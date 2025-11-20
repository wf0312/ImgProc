cmake_minimum_required(VERSION 3.28.0)

project(test VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    include
)
link_directories(
    lib
)

add_executable(${PROJECT_NAME}
    src/cvt_color/from_yuyv.cpp
    src/cvt_color/from_uyvy.cpp
    src/cvt_color/from_rgb.cpp
    src/cvt_color/from_rgba.cpp
    src/image.cpp
    test/test.cpp
    test/cvt_test.cpp
    main.cpp)
