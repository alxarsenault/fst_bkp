#This needs to be included before everything.
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.conan.txt)

cmake_minimum_required(VERSION 3.8.0)

function(make_pretty_source_folders BASE_PATH PREFIX_PATH REQUIRED_ARG)
	foreach(SOURCE IN ITEMS ${REQUIRED_ARG})
		get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
		# message("pretty source_path : ${SOURCE_PATH}")
		file(RELATIVE_PATH SOURCE_PATH_REL "${CMAKE_CURRENT_SOURCE_DIR}/${BASE_PATH}" "${SOURCE_PATH}")
		# message("pretty source_path_rel : ${SOURCE_PATH_REL}")
		string(REPLACE "/" "\\" GROUP_PATH "${PREFIX_PATH}/${SOURCE_PATH_REL}")
		# message("pretty group_path : ${GROUP_PATH}")
		source_group("${GROUP_PATH}" FILES "${SOURCE}")
	endforeach()
endfunction()

#Conan search paths.
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

set(CMAKE_XCODE_GENERATE_SCHEME ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(fst VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

#User options
option(FST_BUILD_TESTS "Build and run tests." ON) 
option(FST_BUILD_BENCH "Build and run benchmarks." ON)

set(FST_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include)

#All sources.
file(GLOB_RECURSE FST_SOURCE_FILES "${FST_INCLUDE_DIRECTORY}/*.h")

#File groups.
make_pretty_source_folders("include/fst" "include" "${FST_SOURCE_FILES}")

add_library(${PROJECT_NAME}  INTERFACE ${FST_SOURCE_FILES})

target_include_directories(${PROJECT_NAME} INTERFACE ${FST_INCLUDE_DIRECTORY})

#Tests
if (${FST_BUILD_TESTS})
    find_package(GTest CONFIG REQUIRED)

    #Test Project
    set(FST_TEST_NAME ${PROJECT_NAME}_tests)
    file(GLOB_RECURSE FST_TEST_SOURCES "tests/*.cpp")

    add_executable(${FST_TEST_NAME} ${FST_TEST_SOURCES})

    target_link_libraries(${FST_TEST_NAME} PUBLIC
        GTest::gtest
        fst
    )

    set(FST_TEST_RESOURCES_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/resources")
    add_definitions(-DFST_TEST_RESOURCES_DIRECTORY="${FST_TEST_RESOURCES_DIRECTORY}")
endif()

#Benchmarks.
if (${FST_BUILD_BENCH})
    find_package(benchmark CONFIG REQUIRED)

    #Test Project
    set(FST_BENCH_NAME ${PROJECT_NAME}_benchs)
    file(GLOB_RECURSE FST_BENCH_SOURCES "bench/*.cpp")

    add_executable(${FST_BENCH_NAME} ${FST_BENCH_SOURCES})

    set_target_properties(${FST_BENCH_NAME} PROPERTIES
        # CXX_STANDARD 17
        # CXX_EXTENSIONS OFF
        # LINKER_LANGUAGE CXX
        CMAKE_CXX_FLAGS "-O3 ${CMAKE_CXX_FLAGS}"
        # FOLDER dev/
    )

    target_compile_definitions(${FST_BENCH_NAME} PUBLIC
        NDEBUG
    )

    target_link_libraries(${FST_BENCH_NAME} PUBLIC
        fst
        benchmark::benchmark
    )
endif()