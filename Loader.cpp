//
// Created by twome on 15/07/2018.
//

#include "Loader.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;


void checkShader(GLuint shader) {
    GLint Result = GL_FALSE;
    int InfoLogLength;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(shader, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        cout << &VertexShaderErrorMessage[0] << endl;
    }
}


const char *readFile(std::string path) {
    printf("Reading file %s\n", path.c_str());
    string *contents = nullptr;
    ifstream fileStream(path.c_str(), ios::in);

    if (fileStream.is_open()) {
        stringstream temp;
        temp << fileStream.rdbuf();
        contents = new string(temp.str());
        fileStream.close();
    }
    if (contents == nullptr) {
        printf("Failed to read file: %s\n", strerror(errno));
        return nullptr;
    }
    return contents->c_str();
}

GLuint Loader::loadShader(std::string shaderName) {
    printf("Loading shader %s\n", shaderName.c_str());
    return loadShader(shaderName + ".v.glsl", shaderName + ".f.glsl");
}

GLuint Loader::loadShader(std::string vertPath, std::string fragPath) {
    std::string shaderBase = "shaders\\";
    const char *vert = readFile(shaderBase + vertPath);
    const char *frag = readFile(shaderBase + fragPath);

    if (vert == nullptr || frag == nullptr) {
        printf("Error: Failed to load shaders\n");
        return 0;
    }


    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShaderId, 1, &vert, nullptr);
    glCompileShader(vertexShaderId);

    checkShader(vertexShaderId);

    glShaderSource(fragmentShaderId, 1, &frag, nullptr);
    glCompileShader(fragmentShaderId);

    checkShader(fragmentShaderId);

    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);

    glDetachShader(programId, vertexShaderId);
    glDetachShader(programId, fragmentShaderId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    printf("Finished loading shader\n");

    return programId;
}
