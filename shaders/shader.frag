#version 330 core

out vec4 fragColour;
in vec3 vertexColour;

void main() {
    fragColour = vec4(vertexColour, 1.0);
}