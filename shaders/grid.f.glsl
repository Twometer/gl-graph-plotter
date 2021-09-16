#version 330 core

uniform vec4 color;
out vec4 out_Colour;

void main(void) {
    out_Colour = color;
}