//
// Created by twome on 15/07/2018.
//

#ifndef GL_GRAPH_PLOTTER_LOADER_H
#define GL_GRAPH_PLOTTER_LOADER_H


#include <string>
#include <glad/glad.h>

class Loader {
public:
    GLuint loadShader(std::string shaderName);

    GLuint loadShader(std::string vertPath, std::string fragPath);
};


#endif //GL_GRAPH_PLOTTER_LOADER_H
