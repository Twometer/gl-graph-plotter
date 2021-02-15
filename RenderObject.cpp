//
// Created by twome on 15 Feb 2021.
//

#include "RenderObject.h"

RenderObject::RenderObject(GLfloat *vertices, int verticesSize) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    numVertices = verticesSize / 2;
}

void RenderObject::Render() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glDrawArrays(GL_LINES, 0, numVertices);
}

RenderObject::~RenderObject() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
