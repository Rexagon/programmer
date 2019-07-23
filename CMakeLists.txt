cmake_minimum_required(VERSION 3.14)
project(programmer)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SYSTEM_BIT_POSTFIX x64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(SYSTEM_BIT_POSTFIX x86)
endif()

if(NOT DEFINED ENV{BOOST_ROOT})
    set(ENV{BOOST_ROOT} C:/boost_1_70_0)
endif()

set(EXTLIBS_DIR ${CMAKE_SOURCE_DIR}/extlibs)
set(BUILD_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build)

# boost
set(Boost_USE_DEBUG_LIBS    OFF)    # не используем дебажные версии
set(Boost_USE_RELEASE_LIBS  ON)     # используем только релизные

set(Boost_ARCHITECTURE      "-x32")

find_package(Boost REQUIRED COMPONENTS system)

# Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# sitl
set(ENV{sitl_DIR} ${EXTLIBS_DIR}/sitl/dist)
message($ENV{sitl_DIR})

find_package(sitl CONFIG REQUIRED)

# application
add_subdirectory(src)
