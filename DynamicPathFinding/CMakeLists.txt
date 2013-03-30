# CMake version control ------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)

# Debug ----------------------------------------------------------------------------------
set(CMAKE_BUILD_TYPE Debug)

# Core Project declaration ---------------------------------------------------------------
project(DynamicPathFinding)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

# Requirements -----------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

# BOOST REQUIREMENTS ---------------------------------------------------------------------
find_package(Boost REQUIRED)
if (Boost_FOUND)
  message("-- Found Boost")
else ()
  message(FATAL_ERROR "-- Unable to Find Boost")
endif()
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

# SFML REQUIREMENTS ----------------------------------------------------------------------
find_package(SFML 1.6 REQUIRED system window graphics network audio)
include_directories(${SFML_INCLUDE_DIR})

# C++11 ----------------------------------------------------------------------------------
if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

# Set core files list --------------------------------------------------------------------
file(
	GLOB
	core_source_files
	sources/*
	GLOB
	core_include_files
	includes/*
)

# add executable for the core BLINK system
add_executable(
	DynamicPathFinding
	${core_include_files}
	${core_source_files}
)

target_link_libraries(DynamicPathFinding ${SFML_LIBRARIES} ${TBB_LIBRARIES})
