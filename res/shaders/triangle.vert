#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 texture_coords;
out vec3 color;
out vec2 texCoords;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main() {
    color = vertex_color;
    texCoords = texture_coords;
    gl_Position = projectionMatrix * modelMatrix * vec4(vertex_position, 1.0);
} 
