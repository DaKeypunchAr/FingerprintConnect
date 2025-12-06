#version 330 core

uniform vec4 uColor;
uniform vec2 uStart;
uniform vec2 uEnd;
uniform float uWidth;

out vec4 color;

float segment_dist(vec2 start, vec2 end, vec2 p) {
    if (start == end) return distance(start, p);
    if (end.x >= start.x && p.x >= end.x) {
        return distance(p, end);
    } else if (end.x <= start.x && p.x <= end.x) {
        return distance(p, end);
    } else if (start.x >= end.x && p.x >= start.x) {
        return distance(p, start);
    } else if (start.x <= end.x && p.x <= start.x) {
        return distance(p, start);
    } else if (end.y >= start.y && p.y >= end.y) {
        return distance(p, end);
    } else if (end.y <= start.y && p.y <= end.y) {
        return distance(p, end);
    } else if (start.y >= end.y && p.y >= start.y) {
        return distance(p, start);
    } else if (start.y <= end.y && p.y <= start.y) {
        return distance(p, start);
    }
    float m = (end.y - start.y) / (end.x - start.x);
    if (m == 0) return distance(p, vec2(p.x, start.y));
    float b = start.y - start.x * m;
    float x = (p.y + (p.x / m) - b) / (m + (1 / m));
    float y = m * x + b;
    vec2 projectionOnLine = vec2(x, y);
    return distance(projectionOnLine, p);
}

void main() {
    float d = segment_dist(uStart, uEnd, gl_FragCoord.xy);
    if (d >= uWidth) {
        discard;
    }
    color = uColor;
}
