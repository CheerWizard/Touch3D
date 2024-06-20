cmake_minimum_required(VERSION 3.22.1)

project(T3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message("CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")
message("CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}")

# defines for build type
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DT3D_DEBUG)
endif ()

include_directories(src)
file(GLOB_RECURSE SRC src/*.cpp)

macro(build_windows)
    add_definitions(-DT3D_WINDOWS)
    add_definitions(-DT3D_DESKTOP)
    file(GLOB_RECURSE SRC_WINDOWS windows/*.cpp)
    file(GLOB_RECURSE SRC_TEST tests/*.cpp)
    include_directories(windows)
    add_executable("T3D_TESTS_WINDOWS" ${SRC} ${SRC_WINDOWS} ${SRC_TEST})
    #target_link_libraries("T3D_TESTS_WINDOWS" vulkan)
    add_executable(${PROJECT_NAME} WIN32 ${SRC} ${SRC_WINDOWS} WindowsMain.cpp)
    #target_link_libraries(${PROJECT_NAME} vulkan)
endmacro()

macro(build_android)
    add_definitions(-DT3D_ANDROID)
    add_definitions(-DT3D_PHONE)
    add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR=1)
    file(GLOB_RECURSE SRC_ANDROID android/*.cpp)
    file(GLOB_RECURSE SRC_TEST tests/*.cpp)
    include_directories(android)
    add_library("T3D_TESTS_ANDROID" SHARED ${SRC} ${SRC_ANDROID} ${SRC_TEST})
    target_link_libraries("T3D_TESTS_ANDROID"
            android
            log
            #vulkan
    )
    add_library(${PROJECT_NAME} SHARED ${SRC} ${SRC_ANDROID} AndroidMain.cpp)
    target_link_libraries(${PROJECT_NAME}
            android
            log
            #vulkan
    )
endmacro()

macro(build_linux)
    add_definitions(-DT3D_LINUX)
    add_definitions(-DT3D_DESKTOP)
    file(GLOB_RECURSE SRC_LINUX linux/*.cpp)
    file(GLOB_RECURSE SRC_TEST tests/*.cpp)
    include_directories(linux)
    add_executable("T3D_TESTS_LINUX" ${SRC} ${SRC_LINUX} ${SRC_TEST})
    target_link_libraries("T3D_TESTS_LINUX" X11)
    add_executable(${PROJECT_NAME} ${SRC} ${SRC_LINUX} LinuxMain.cpp)
    target_link_libraries(${PROJECT_NAME} X11)
endmacro()

# defines for OS
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    build_windows()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
    build_android()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    build_linux()
endif ()

# defines for CPU arch
if (
    ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ilp32"
    OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ILP32"
    OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm"
    OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM"
    OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i386"
    OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "I386"
    OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ix86"
    OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "IX86"
    OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86"
    OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "X86"
    OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "powerpc"
    OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "armv7-a"
    OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686"
)
    add_definitions(-DT3D_32)
else ()
    add_definitions(-DT3D_64)
endif()