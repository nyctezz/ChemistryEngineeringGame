#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture; // The texture sampler

void main() {
    FragColor = texture(ourTexture, TexCoord); // Samples the color from your PNG
}