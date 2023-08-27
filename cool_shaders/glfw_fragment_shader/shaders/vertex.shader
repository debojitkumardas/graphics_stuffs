#version 330 core

// const vec2 quad_vertices[4] = vec2[4](vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(1.0, 1.0), vec2(-1.0, 1.0));
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0F);  // vec4(quad_vertices[gl_VertexID], 0.0, 1.0);
}
