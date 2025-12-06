#version 330 core

in vec2 texCoords;
uniform sampler2D uTexture;
uniform vec4 uColor;

out vec4 color;

void main() {
    float intensity = texture(uTexture, texCoords).r;
    color = vec4(uColor.xyz, uColor.w * intensity);
}
