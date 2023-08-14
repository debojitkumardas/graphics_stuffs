#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertex_color;

void main() {
    gl_Position = vec4(aPos, 1.0F);
    vertex_color = aColor; // vec4(0.5, 0.0, 0.0, 1.0);
}
