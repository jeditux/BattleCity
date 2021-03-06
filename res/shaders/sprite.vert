#version 460
layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 texture_coords;
out vec2 texCoords;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;

void main() {
    texCoords = texture_coords;
    gl_Position = projectionMatrix * modelMatrix * vec4(vertex_position, 0.0, 1.0);
} 
