#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform vec2 uTranslation;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 TexCoord;

void main() {
    vec4 worldPosition = vec4(aPos.xy + uTranslation, aPos.z, 1.0);
    gl_Position = uProjection * uView * worldPosition;
    TexCoord = aTexCoord;
}
