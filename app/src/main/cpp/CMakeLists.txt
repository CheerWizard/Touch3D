cmake_minimum_required(VERSION 3.22.1)

project(T3D)

option(T3D_BUILD_WINDOWS "Build for Windows platform" OFF)
option(T3D_BUILD_ANDROID "Build for Android platform" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u MainActivity_onCreate")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DT3D_DEBUG)
endif ()

file(GLOB_RECURSE SOURCES ../cpp/*.cpp ../cpp/*.c)

include_directories(../cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (T3D_BUILD_WINDOWS)
    target_link_libraries(${PROJECT_NAME}
            vulkan
    )
endif ()

if (T3D_BUILD_ANDROID)
    target_link_libraries(${PROJECT_NAME}
            android
            log
            EGL
            GLESv1_CM
            vulkan
    )
endif ()