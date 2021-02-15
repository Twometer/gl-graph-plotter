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


#define WINDOW_WIDTH 1024.0f
#define WINDOW_HEIGHT 768.0f
#define GRID_SPACING 32.0f

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
        return 0;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    Loader loader = Loader();

    GLuint gridShader = loader.loadShader("grid");
    GLint matLocation = glGetUniformLocation(gridShader, "projectionMatrix");
    GLint xOffLocation = glGetUniformLocation(gridShader, "xOff");
    GLint yOffLocation = glGetUniformLocation(gridShader, "yOff");

    auto *gridVertices = new GLfloat[5000];
    int gridVerticesAlloc = 0;

    for (GLfloat i = -GRID_SPACING; i <= WINDOW_WIDTH + GRID_SPACING; i += GRID_SPACING) {
        // Vertical
        gridVertices[gridVerticesAlloc] = i;
        gridVertices[gridVerticesAlloc + 1] = -GRID_SPACING;
        gridVertices[gridVerticesAlloc + 2] = i;
        gridVertices[gridVerticesAlloc + 3] = WINDOW_HEIGHT + GRID_SPACING;

        gridVerticesAlloc += 4;

        // Horizontal
        gridVertices[gridVerticesAlloc] = -GRID_SPACING;
        gridVertices[gridVerticesAlloc + 1] = i;
        gridVertices[gridVerticesAlloc + 2] = WINDOW_WIDTH * GRID_SPACING;
        gridVertices[gridVerticesAlloc + 3] = i;

        gridVerticesAlloc += 4;
    }

    auto *graphVertices = new GLfloat[WINDOW_WIDTH * 4];
    for (int i = 0; i < WINDOW_WIDTH; i++) {
        graphVertices[i] = i;
        graphVertices[i + 1] = 0;
        graphVertices[i + 2] = i + 1;
        graphVertices[i + 3] = 0;
    }

    RenderObject grid(gridVertices, gridVerticesAlloc);
    RenderObject graph(graphVertices, WINDOW_WIDTH * 4);

    glm::mat4 orthoMat = glm::ortho(0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f);

    float xoff = 0.0f;
    float yoff = 0.0f;

    float virtualX = 0.0f;
    float virtualY = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(gridShader);
        glUniformMatrix4fv(matLocation, 1, GL_FALSE, &orthoMat[0][0]);
        glUniform1f(xOffLocation, xoff);
        glUniform1f(yOffLocation, yoff);
        grid.Render();

        glUniform1f(xOffLocation, virtualX);
        glUniform1f(yOffLocation, virtualY);
        graph.Render();

        static double lmx, lmy;
        double mx, my;
        glfwGetCursorPos(window, &mx, &my);

        double dx = mx - lmx;
        double dy = my - lmy;

        lmx = mx;
        lmy = my;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            xoff += dx;
          //  if (xoff < -GRID_SPACING) xoff = 0;
           // if (xoff > GRID_SPACING) xoff = 0;

            yoff += dy;
          //  if (yoff < -GRID_SPACING) yoff = 0;
          //  if (yoff > GRID_SPACING) yoff = 0;

            virtualX += dx;
            virtualY += dy;
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(gridShader);

    return 0;
}
