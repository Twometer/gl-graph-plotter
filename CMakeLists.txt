cmake_minimum_required(VERSION 3.10)
project(gl_graph_plotter)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(${CMAKE_SOURCE_DIR}/glm)

include_directories(${CMAKE_SOURCE_DIR}/inc)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(gl_graph_plotter
        main.cpp
        Loader.cpp
        RenderObject.cpp

        glad.c
        inc/imgui/imgui_impl_opengl3.cpp
        inc/imgui/imgui.cpp
        inc/imgui/imgui_tables.cpp
        inc/imgui/imgui_widgets.cpp
        inc/imgui/imgui_demo.cpp
        inc/imgui/imgui_draw.cpp
        inc/imgui/imgui_impl_glfw.cpp ImguiStyle.h)