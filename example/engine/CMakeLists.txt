cmake_minimum_required(VERSION 3.2.1)
project(engine)

# Use folder.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE ENGINE_INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include/engine/*.hpp)
file(GLOB_RECURSE ENGINE_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/source/engine/*.cpp)

file(GLOB_RECURSE GAME_INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include/game/*.hpp)
file(GLOB_RECURSE GAME_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/source/game/*.cpp)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

source_group("include\\engine" FILES ${ENGINE_INCLUDE_FILES})
source_group("source\\engine" FILES ${ENGINE_SOURCE_FILES})
source_group("include\\game" FILES ${GAME_INCLUDE_FILES})
source_group("source\\game" FILES ${GAME_SOURCE_FILES})
source_group("source" FILES ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${ENGINE_INCLUDE_FILES} ${ENGINE_SOURCE_FILES} ${GAME_INCLUDE_FILES} ${GAME_SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include) 
target_include_directories(${PROJECT_NAME} PUBLIC ${FST_INCLUDE_DIRECTORY}) 
target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/include)

link_directories(/usr/local/lib/)
target_link_libraries(${PROJECT_NAME} "-framework Cocoa -framework OpenGL -framework CoreVideo -framework IOKit")
target_link_libraries(${PROJECT_NAME} /usr/local/lib/libglfw3.a)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER example/)