#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colour;
layout (location = 2) in vec2 texCoordIn;

out vec3 vertexColour;
out vec2 texCoord;

void main() {
    gl_Position = vec4(position, 1.0);
    vertexColour = colour;
    texCoord = texCoordIn;
}