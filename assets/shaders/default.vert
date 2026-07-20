#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform vec2 uTranslation;

out vec2 TexCoord;

void main() {
    vec2 position = aPos.xy + uTranslation;
    gl_Position = vec4(position, aPos.z, 1.0);
    TexCoord = aTexCoord;
}
