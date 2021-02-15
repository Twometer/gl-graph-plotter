#version 330 core

layout(location = 0) in vec2 position;

uniform mat4 projectionMatrix;
uniform float xOff;
uniform float yOff;

void main(void) {
    gl_Position = projectionMatrix * vec4(position + vec2(xOff, yOff), 0.0, 1.0);
}