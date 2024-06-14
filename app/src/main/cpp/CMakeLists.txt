cmake_minimum_required(VERSION 3.22.1)

project(T3D)

option(T3D_TEST_MATH "Tests for Math" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DT3D_DEBUG)
endif ()

include_directories(src)
file(GLOB_RECURSE SRC src/*.cpp)

macro(build_windows)
    add_definitions(-DT3D_WINDOWS)
    add_definitions(-DT3D_DESKTOP)
    file(GLOB_RECURSE WINDOWS_SRC windows/*.cpp)
    include_directories(windows)
    if (T3D_TEST_MATH)
        add_executable(${PROJECT_NAME} ${SRC} ${WINDOWS_SRC} TestMath.cpp)
#        target_link_libraries(${PROJECT_NAME} vulkan)
    else ()
        add_executable(${PROJECT_NAME} WIN32 ${SRC} ${WINDOWS_SRC} WindowsMain.cpp)
#        target_link_libraries(${PROJECT_NAME} vulkan)
    endif ()
endmacro()

macro(build_android)
    add_definitions(-DT3D_ANDROID)
    add_definitions(-DT3D_PHONE)
    file(GLOB_RECURSE ANDROID_SRC android/*.cpp)
    include_directories(android)
    add_library(${PROJECT_NAME} SHARED ${SRC} ${ANDROID_SRC} AndroidMain.cpp)
    target_link_libraries(${PROJECT_NAME}
            android
            log
            vulkan
    )
endmacro()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    build_windows()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
    build_android()
endif ()