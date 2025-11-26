#version 330 core

in vec2 texCoords;
uniform sampler2D tex;

out vec4 color;

void main() {
    vec4 col = vec4(0.3, 0.2, 0.1, 1.0);
    float intensity = texture(tex, texCoords).r;
    color = vec4(col.xyz, col.w * intensity);
}
