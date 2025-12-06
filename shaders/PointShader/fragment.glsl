#version 330 core

uniform vec4 uColor;
uniform float uRadius;
uniform vec2 uPoint;

out vec4 color;

void main() {
    float d = distance(uPoint, gl_FragCoord.xy);
    if (d >= uRadius) {
        discard;
    }
    color = uColor;
}
