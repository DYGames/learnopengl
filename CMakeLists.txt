cmake_minimum_required(VERSION 3.10)

project(OPENGLTEST)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(OPENGLTEST src/main.cpp)

find_package(OpenGL REQUIRED)
target_link_libraries(OPENGLTEST OpenGL::GL)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(third-party/glfw-3.3.8)
target_link_libraries(OPENGLTEST glfw)

# GLAD
set(glad_path third-party/glad)
add_library(glad STATIC 
    "${glad_path}/include/glad/glad.h" 
    "${glad_path}/include/KHR/khrplatform.h" 
    "${glad_path}/src/glad.c")
target_include_directories(glad PUBLIC "${glad_path}/include")
target_link_libraries(OPENGLTEST glad)
