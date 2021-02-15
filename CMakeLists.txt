cmake_minimum_required(VERSION 3.10)
project(gl_graph_plotter)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(${CMAKE_SOURCE_DIR}/glm)

include_directories(${CMAKE_SOURCE_DIR}/inc)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(gl_graph_plotter main.cpp glad.c Loader.cpp Loader.h RenderObject.cpp RenderObject.h)