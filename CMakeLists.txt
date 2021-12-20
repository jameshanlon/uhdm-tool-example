cmake_minimum_required(VERSION 3.21.0)
project(uhdm-tool-example)
include(ExternalProject)
include(GNUInstallDirs)

# Boost
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.68.0 REQUIRED COMPONENTS
             unit_test_framework)

# Python
find_package(Python3 REQUIRED COMPONENTS
             Interpreter)

# Compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -pedantic)

ExternalProject_Add(SureLog
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/UHDM
  PREFIX UHDM
  CONFIGURE_COMMAND cmake <SOURCE_DIR> -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
  BUILD_COMMAND make
  BUILD_ALWAYS 1
  INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
  INSTALL_COMMAND make install)

add_library(libcapnp STATIC IMPORTED GLOBAL)
add_library(libkj STATIC IMPORTED GLOBAL)
add_library(libuhdm STATIC IMPORTED GLOBAL)

set_target_properties(libcapnp PROPERTIES
  IMPORTED_LOCATION ${CMAKE_INSTALL_PREFIX}/lib/uhdm/libcapnp.a)

set_target_properties(libkj PROPERTIES
  IMPORTED_LOCATION ${CMAKE_INSTALL_PREFIX}/lib/uhdm/libkj.a)

set_target_properties(libuhdm PROPERTIES
  IMPORTED_LOCATION ${CMAKE_INSTALL_PREFIX}/lib/uhdm/libuhdm.a)

include_directories(${CMAKE_INSTALL_PREFIX}/include/uhdm
                    ${CMAKE_INSTALL_PREFIX}/include/surelog
                    ${Boost_INCLUDE_DIRS})

add_executable(main main.cpp)
target_include_directories(main PUBLIC
  ${CMAKE_INSTALL_PREFIX}/include/uhdm)
target_link_libraries(main libkj libcapnp libuhdm ${Boost_LIBRARIES})

install(TARGETS main DESTINATION ${CMAKE_INSTALL_BINDIR})
