

cmake_minimum_required(VERSION 3.10)

set(OBJECT_GL_LIBRARY_NAME object-gl)

project(${OBJECT_GL_LIBRARY_NAME})

find_package(fmt REQUIRED)
find_package(stb REQUIRED)

add_library(${OBJECT_GL_LIBRARY_NAME} ./src/gl.cpp ./src/egl.cpp)

target_compile_features(${OBJECT_GL_LIBRARY_NAME} PUBLIC cxx_std_17)

target_link_libraries(${OBJECT_GL_LIBRARY_NAME} PUBLIC fmt::fmt stb::stb raii GLESv2 EGL)

target_include_directories(${OBJECT_GL_LIBRARY_NAME} PUBLIC ./include)
