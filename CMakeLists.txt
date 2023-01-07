cmake_minimum_required(VERSION 3.16)
project(CONIXPP)

add_definitions("-std=c++14 -Wall")

set(CMAKE_CXX_STANDARD 14)
set(CONIXPP_PUBLIC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CONIXPP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB CONIXPP_PUBLIC "${CONIXPP_PUBLIC_DIR}/*.hpp")
file(GLOB CONIXPP_SOURCES "${CONIXPP_SOURCE_DIR}/*.cpp")

include_directories(${CONIXPP_PUBLIC_DIR})
include_directories(${CONIXPP_SOURCE_DIR})

add_library(conixpp ${CONIXPP_SOURCES})

add_executable(demo demo/main.cpp)
target_link_libraries(demo conixpp)

install(FILES ${CONIXPP_PUBLIC} DESTINATION include)
install(TARGETS conixpp ARCHIVE DESTINATION lib)
