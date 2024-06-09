cmake_minimum_required(VERSION 3.22.1)

project(T3D)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DT3D_DEBUG)
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_definitions(-DT3D_WINDOWS)
    add_definitions(-DT3D_DESKTOP)
    file(GLOB_RECURSE SOURCES src/windows/*.cpp)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_definitions(-DT3D_LINUX)
    add_definitions(-DT3D_DESKTOP)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_definitions(-DT3D_MACOS)
    add_definitions(-DT3D_DESKTOP)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
    add_definitions(-DT3D_ANDROID)
    add_definitions(-DT3D_PHONE)
    file(GLOB_RECURSE SOURCES src/android/*.cpp)
endif ()

include_directories(src)
include_directories(vendor)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (T3D_BUILD_WINDOWS)
    target_link_libraries(${PROJECT_NAME}
            vulkan
    )
elseif (T3D_BUILD_ANDROID)
    target_link_libraries(${PROJECT_NAME}
            android
            log
            EGL
            GLESv1_CM
            vulkan
    )
endif ()