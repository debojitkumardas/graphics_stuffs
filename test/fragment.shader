#version 330 core
out vec4 FragColor;
in vec3 vertex_color;

// uniform vec4 vertex_color;

void main() {
    FragColor = vec4(vertex_color, 1.0); // vec4(1.0F, 0.5F, 0.2F, 1.0F);
}
