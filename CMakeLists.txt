cmake_minimum_required(VERSION 3.9)
project(
	ConixPP
	VERSION 0.0.0
	DESCRIPTION "A library for easy handling of command line options for C++"
	LANGUAGES CXX
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ standard to use")
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)

	file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
	if(EXISTS "${LOC_PATH}")
		message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
	endif()

	include(CheckIPOSupported)
	check_ipo_supported(RESULT IPO_SUPPORTED)
	if(IPO_SUPPORTED)
		message(STATUS "Interprocedural optimization supported")
	else()
		message(STATUS "Interprocedural optimization not supported")
	endif()

	if(MSVC)
		add_compile_options(/W4 /WX)
	else()
		add_compile_options(-Wall -Wextra -pedantic -Werror)
	endif()
endif()

set(CONIXPP_PUBLIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(CONIXPP_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(CONIXPP_DEMO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/demo")

file(GLOB CONIXPP_PUBLIC "${CONIXPP_PUBLIC_DIR}/*.hpp" "${CONIXPP_PUBLIC_DIR}/**/*.hpp")
file(GLOB CONIXPP_SOURCES "${CONIXPP_SOURCE_DIR}/*.cpp" "${CONIXPP_SOURCE_DIR}/**/*.cpp")
file(GLOB CONIXPP_DEMO_SOURCES "${CONIXPP_DEMO_DIR}/*.cpp" "${CONIXPP_DEMO_DIR}/**/*.cpp")

add_library(conixpp "${CONIXPP_SOURCES}" "${CONIXPP_TOOLS}")
target_include_directories(conixpp PRIVATE "${CONIXPP_SOURCE_DIR}")
target_include_directories(conixpp PUBLIC "${CONIXPP_PUBLIC_DIR}")
if(IPO_SUPPORTED)
  set_target_properties(conixpp PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

add_executable(demo "${CONIXPP_DEMO_SOURCES}")
target_include_directories(demo PRIVATE "${CONIXPP_PUBLIC_DIR}")
target_include_directories(demo PRIVATE "${CONIXPP_DEMO_DIR}")
target_link_libraries(demo conixpp)

install(FILES ${CONIXPP_PUBLIC} DESTINATION include/conixpp)
install(TARGETS conixpp ARCHIVE DESTINATION lib)
