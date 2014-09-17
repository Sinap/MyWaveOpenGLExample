project(wave)
cmake_minimum_required(VERSION 2.8)

add_definitions(-std=c11)

# get all .c files
file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.c")
file(GLOB HRD_FILES "${PROJECT_SOURCE_DIR}/src/*.h")

# create wave executable
add_executable(wave ${SRC_FILES} ${HRD_FILES})

# don't build docs, tests, or examples from glfw
set(GLFW_BUILD_DOCS "")
set(GLFW_BUILD_EXAMPLES "")
set(GLFW_BUILD_TESTS "")

# add glfw CMakeLists
add_subdirectory("${PROJECT_SOURCE_DIR}/deps/glfw")

# add libraries to compiler search path
include_directories("${GLFW_SOURCE_DIR}/include")

if(APPLE)
    target_link_libraries(wave glfw ${GLFW_LIBRARIES})
endif(APPLE)

if(UNIX)
    target_link_libraries(wave dl glfw ${GLFW_LIBRARIES})
endif()
