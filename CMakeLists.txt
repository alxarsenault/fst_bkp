cmake_minimum_required(VERSION 3.2.1)

# Check if we run cmake at root of workspace.
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Source and build directories cannot be the same.")
endif ()

project(fst)

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
file(GLOB FOLDER_FILE_FST_IO_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include/fst/io/*.h)
file(GLOB FOLDER_FILE_FST_IO_STRM_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include/fst/io/stream/*.h)

source_group("include" FILES ${FOLDER_FILE_FST_HEADER_FILES})
source_group("include\\io" FILES ${FOLDER_FILE_FST_IO_HEADER_FILES})
source_group("include\\io\\stream" FILES ${FOLDER_FILE_FST_IO_STRM_HEADER_FILES})
# message("FST_HEADER_FILES : " ${FST_HEADER_FILES})

add_custom_target(${PROJECT_NAME} SOURCES ${FST_HEADER_FILES})

install(FILES ${FST_HEADER_FILES} DESTINATION /usr/local/include/fst/)

# Examples.
add_subdirectory(example/print)
add_subdirectory(example/visitor)
add_subdirectory(example/multi_vector)
add_subdirectory(example/vector)
add_subdirectory(example/one_file_engine)
add_subdirectory(example/engine)
add_subdirectory(example/event)
