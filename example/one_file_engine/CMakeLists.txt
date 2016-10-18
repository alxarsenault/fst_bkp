cmake_minimum_required(VERSION 3.2.1)
project(one_file_engine)

# Use folder.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

source_group("source" FILES ${SOURCE_FILES})
source_group("include" FILES ${INCLUDE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include) 
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/cocoa) 
target_include_directories(${PROJECT_NAME} PUBLIC ${FST_INCLUDE_DIRECTORY}) 
target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/include)

link_directories(/usr/local/lib/)
target_link_libraries(${PROJECT_NAME} "-framework Cocoa -framework OpenGL -framework CoreVideo -framework IOKit")
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libglfw3.a)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER example/)