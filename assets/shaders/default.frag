#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture; // The texture sampler
uniform vec4 uColorTint = vec4(1.0, 1.0, 1.0, 1.0); // Default to full opacity

void main() 
{
    FragColor = texture(ourTexture, TexCoord) * uColorTint;
}