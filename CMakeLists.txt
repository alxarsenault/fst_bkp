cmake_minimum_required(VERSION 3.5.0)

# Check if we run cmake at root of workspace.
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Source and build directories cannot be the same.")
endif ()

project(fstLib)
set(FST_VERSION 0.0.1)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(BUILD_EXAMPLES "Build with examples" OFF)

# Use folder.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(FST_INCLUDE_DIRECTORY ${CMAKE_SOURCE_DIR}/include/)

# Debug output path.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Debug)

# Release output path.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Release)

# Set debug as default build type.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CONFIG_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Release)
else ()
    set(CONFIG_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${CMAKE_SYSTEM_PROCESSOR}/Debug)
endif ()

# Compiler flags.
set(CMAKE_CXX_FLAGS "-fPIC -std=c++14 -Wall -Wextra ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 ${CMAKE_CXX_FLAGS_RELEASE}")

file(GLOB_RECURSE FST_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include/fst/*.h)
file(GLOB FOLDER_FILE_FST_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include/fst/*.h)

source_group("include" FILES ${FOLDER_FILE_FST_HEADER_FILES})

# Dummy project.
# add_library(fst_header STATIC ${FST_HEADER_FILES})
# set_target_properties(fst_header PROPERTIES LINKER_LANGUAGE CXX)

# Install Package Configuration

set(FST_INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/fst")
install(FILES ${FST_HEADER_FILES} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/fst")

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/fstConfigVersion.cmake"
	VERSION ${FST_VERSION}
	COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
	cmake/fstConfig.cmake.in
	${FST_INSTALL_CONFIGDIR}/fstConfig.cmake
	INSTALL_DESTINATION ${FST_INSTALL_CONFIGDIR}
	PATH_VARS CMAKE_INSTALL_INCLUDEDIR
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(
	FILES
		"${CMAKE_CURRENT_BINARY_DIR}/${FST_INSTALL_CONFIGDIR}/fstConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/fstConfigVersion.cmake"
	DESTINATION ${FST_INSTALL_CONFIGDIR}
)

add_library(fst INTERFACE)
target_include_directories(fst INTERFACE
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
install(TARGETS fst EXPORT fstTargets)

export(EXPORT fstTargets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/fstTargets.cmake"
)

install(
	EXPORT fstTargets FILE fstTargets.cmake
	DESTINATION ${FST_INSTALL_CONFIGDIR}
)

export(PACKAGE fst)

# Examples.
if(BUILD_EXAMPLES)
	add_subdirectory(example/print)
	# add_subdirectory(example/visitor)
	# add_subdirectory(example/multi_vector)
	add_subdirectory(example/multi_vector2)
	add_subdirectory(example/vector)
	add_subdirectory(example/array)
	# add_subdirectory(example/serialize)
	# BROKEN add_subdirectory(example/one_file_engine)
	#add_subdirectory(example/engine)
	add_subdirectory(example/event)
	add_subdirectory(example/fstring)
	add_subdirectory(example/heap_aligned_storage)
endif(BUILD_EXAMPLES)
