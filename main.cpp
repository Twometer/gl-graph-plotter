#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "RenderObject.h"

#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "OpenGL32.lib")

using namespace std;

#define WINDOW_WIDTH 1024.f
#define WINDOW_HEIGHT 768.f
#define GRID_SPACING 32

GLuint generateShader(const std::string &formula) {
    auto fragment = "#version 330 core\n"
                    "\n"
                    "uniform vec4 color;\n"
                    "out vec4 out_Colour;\n"
                    "\n"
                    "void main(void) {\n"
                    "    out_Colour = color;\n"
                    "}";

    auto vertex = "#version 330 core\n"
                  "#define PI 3.14159265359\n"
                  "#define E  2.71828182845\n"
                  "\n"
                  "layout(location = 0) in vec2 position;\n"
                  "\n"
                  "uniform mat4 projectionMatrix;\n"
                  "uniform float xOff;\n"
                  "uniform float yOff;\n"
                  "uniform float gridSize;"
                  "\n"
                  "float f(float x) {\n"
                  "return " + formula + ";\n"
                                        "}\n"
                                        "void main(void) {\n"
                                        "    float func_val = f((position.x - xOff) / gridSize) * gridSize;\n"
                                        "    gl_Position = projectionMatrix * vec4(position + vec2(0, yOff + func_val), 0.0, 1.0);\n"
                                        "}";


    Loader loader{};
    return loader.loadShaderRaw(vertex.c_str(), fragment);
}

int main() {
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Graph plotter", nullptr, nullptr);
    if (!window) {
        cout << "Window could not be created" << endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "Failed to load OpenGL" << endl;
        glfwTerminate();
        return 1;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);



    Loader loader;

    GLuint graphShader = generateShader("sin(x) + 0.1 * x");
    GLuint gridShader = loader.loadShader("grid");
    GLint grid_matLocation = glGetUniformLocation(gridShader, "projectionMatrix");
    GLint grid_xOffLocation = glGetUniformLocation(gridShader, "xOff");
    GLint grid_yOffLocation = glGetUniformLocation(gridShader, "yOff");
    GLint grid_colorLocation = glGetUniformLocation(gridShader, "color");

    GLint graph_matLocation = glGetUniformLocation(graphShader, "projectionMatrix");
    GLint graph_xOffLocation = glGetUniformLocation(graphShader, "xOff");
    GLint graph_yOffLocation = glGetUniformLocation(graphShader, "yOff");
    GLint graph_colorLocation = glGetUniformLocation(graphShader, "color");
    GLint graph_gridSizeLocation = glGetUniformLocation(graphShader, "gridSize");

    auto *gridVertices = new GLfloat[2048];
    int gridVerticesAlloc = 0;

    // Vertical
    for (int i = -GRID_SPACING; i <= WINDOW_WIDTH + GRID_SPACING; i += GRID_SPACING) {
        gridVertices[gridVerticesAlloc] = (float) i;
        gridVertices[gridVerticesAlloc + 1] = -GRID_SPACING;
        gridVertices[gridVerticesAlloc + 2] = (float) i;
        gridVertices[gridVerticesAlloc + 3] = WINDOW_HEIGHT + GRID_SPACING;

        gridVerticesAlloc += 4;
    }

    // Horizontal
    for (int i = -GRID_SPACING; i <= WINDOW_HEIGHT + GRID_SPACING; i += GRID_SPACING) {
        gridVertices[gridVerticesAlloc] = -GRID_SPACING;
        gridVertices[gridVerticesAlloc + 1] = (float) i;
        gridVertices[gridVerticesAlloc + 2] = WINDOW_WIDTH + GRID_SPACING;
        gridVertices[gridVerticesAlloc + 3] = (float) i;

        gridVerticesAlloc += 4;
    }

    auto *graphVertices = new GLfloat[4096];
    int graphVerticesAlloc = 0;
    for (int i = 0; i < WINDOW_WIDTH; i += 2) {
        graphVertices[i + 0] = i;
        graphVertices[i + 1] = 0;
        graphVerticesAlloc += 2;
    }

    RenderObject grid(gridVertices, gridVerticesAlloc, GL_LINES);
    RenderObject graph(graphVertices, graphVerticesAlloc, GL_LINE_STRIP);

    float crossVertices[] = {-10000, 0, 10000, 0, 0, -10000, 0, 10000};
    RenderObject cross(crossVertices, 8, GL_LINES);

    glm::mat4 orthoMat = glm::ortho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT);

    int xoff = 0;
    int yoff = 0;

    int virtualX = WINDOW_WIDTH / 2;
    int virtualY = WINDOW_HEIGHT / 2;


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(gridShader);
        glUniformMatrix4fv(grid_matLocation, 1, GL_FALSE, &orthoMat[0][0]);
        glUniform1f(grid_xOffLocation, xoff);
        glUniform1f(grid_yOffLocation, yoff);
        glUniform4f(grid_colorLocation, 0.75, 0.75, 0.75, 0.45f);
        grid.Render();

        glUniform1f(grid_xOffLocation, virtualX);
        glUniform1f(grid_yOffLocation, virtualY);
        glUniform4f(grid_colorLocation, 0, 0, 0, 1.0f);
        cross.Render();

        glUseProgram(graphShader);
        glUniformMatrix4fv(graph_matLocation, 1, GL_FALSE, &orthoMat[0][0]);
        glUniform1f(graph_xOffLocation, virtualX);
        glUniform1f(graph_yOffLocation, virtualY);
        glUniform4f(graph_colorLocation, 1.0, 0.25, 0.25, 1.0f);
        glUniform1f(graph_gridSizeLocation, GRID_SPACING);
        graph.Render();

        static double lmx, lmy;
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);

        int dx = (int) (mx - lmx);
        int dy = -(int) (my - lmy);

        lmx = mx;
        lmy = my;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            xoff += dx;
            xoff %= GRID_SPACING;

            yoff += dy;
            yoff %= GRID_SPACING;

            virtualX += dx;
            virtualY += dy;
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(gridShader);

    return 0;
}