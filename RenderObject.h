//
// Created by twome on 15 Feb 2021.
//

#ifndef GL_GRAPH_PLOTTER_RENDEROBJECT_H
#define GL_GRAPH_PLOTTER_RENDEROBJECT_H

#include <glad/glad.h>

class RenderObject {
private:
    GLuint vao;
    GLuint vbo;
    int numVertices;
    int primitiveType;

public:
    RenderObject(GLfloat *vertices, int verticesSize, int primitiveType);

    ~RenderObject();

    void Render();
};


#endif //GL_GRAPH_PLOTTER_RENDEROBJECT_H
