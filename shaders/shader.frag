#version 330 core

out vec4 fragColour;

in vec3 vertexColour;
in vec2 texCoord;

uniform sampler2D customTexture;

void main() {
    fragColour = texture(customTexture, texCoord);
}