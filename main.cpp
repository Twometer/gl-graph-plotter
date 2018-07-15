#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Loader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "OpenGL32.lib")

using namespace std;

int main() {

    if (!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Graph plotter", nullptr, nullptr);
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

    GLuint coordsShader = loader.loadShader("coords");
    GLint matLocation = glGetUniformLocation(coordsShader, "projectionMatrix");

    GLfloat *vertices = new GLfloat[4096 * 2];
    int verticesAlloc = 0;
    GLfloat *colors = new GLfloat[4096 * 3];
    int colorsAlloc = 0;

    for (GLfloat i = 0; i <= 800.0f; i += 16.0f) {
        vertices[verticesAlloc] = i;
        vertices[verticesAlloc + 1] = 0;
        vertices[verticesAlloc + 2] = i;
        vertices[verticesAlloc + 3] = 600;

        colors[colorsAlloc] = 0;
        colors[colorsAlloc + 1] = 0;
        colors[colorsAlloc + 2] = 0;
        colors[colorsAlloc + 3] = 0;
        colors[colorsAlloc + 4] = 0;
        colors[colorsAlloc + 5] = 0;

        verticesAlloc += 4;
        colorsAlloc += 6;
    }

    GLuint mainVao;
    glGenVertexArrays(1, &mainVao);
    glBindVertexArray(mainVao);
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, verticesAlloc * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, colorsAlloc * sizeof(GLfloat), colors, GL_STATIC_DRAW);

    glm::mat4 orthoMat = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(coordsShader);
        glUniformMatrix4fv(matLocation, 1, GL_FALSE, &orthoMat[0][0]);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *) 0);

        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

        glDrawArrays(GL_LINES, 0, verticesAlloc / 2);

        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &mainVao);
    glDeleteProgram(coordsShader);

    return 0;
}
