cmake_minimum_required(VERSION 3.14)
project(programmer)

option(WIN_DEPLOY "Find and deploy all dlls for windows (works only on windows)" ON)

if (NOT DEFINED ENV{APP_VERSION})
    set(ENV{APP_VERSION} "DEV")
endif()

message("Building programmer: $ENV{APP_VERSION}")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SYSTEM_BIT_POSTFIX x64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(SYSTEM_BIT_POSTFIX x86)
endif()

if(NOT DEFINED ENV{BOOST_ROOT})
    set(ENV{BOOST_ROOT} C:/boost_1_70_0)
endif()

set(EXTLIBS_DIR ${CMAKE_SOURCE_DIR}/3rdparty)
set(BUILD_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build)

# boost
if (NOT DEFINED ENV{BOOST_ROOT})
    set(ENV{BOOST_ROOT} C:/boost_1_70_0)
endif()

set(Boost_USE_DEBUG_LIBS    OFF)    # не используем дебажные версии
set(Boost_USE_RELEASE_LIBS  ON)     # используем только релизные

set(Boost_ARCHITECTURE      "-x32")

find_package(Boost REQUIRED COMPONENTS system)

# Qt
if (NOT DEFINED ENV{Qt5_DIR})
    set(ENV{Qt5_DIR} C:/Qt/Qt5.12.3/5.12.3/mingw73_32/lib/cmake/Qt5)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets SerialPort REQUIRED)

# sitl
if (NOT DEFINED ENV{sitl_DIR})
    set(ENV{sitl_DIR} ${EXTLIBS_DIR}/sitl/dist)
endif()

find_package(sitl CONFIG REQUIRED)

# application
add_subdirectory(src)
