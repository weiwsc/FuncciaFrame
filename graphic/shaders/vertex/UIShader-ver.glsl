#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec4 a_borderBox;        // x1, y1, x2, y2
layout (location = 2) in vec4 a_backgroundColor;  // r, g, b, a
layout (location = 3) in vec4 a_borderRadius;     // tl, tr, br, bl

uniform mat4 u_viewProjection;

out vec2 v_pixelPos;
out vec4 v_borderBox;
out vec4 v_backgroundColor;
out vec4 v_borderRadius;

void main() {
    vec2 size = a_borderBox.zw - a_borderBox.xy;
    vec2 worldPos = a_borderBox.xy + a_position * size;

    gl_Position = u_viewProjection * vec4(worldPos, 0.0, 1.0);

    v_pixelPos = worldPos;
    v_borderBox = a_borderBox;
    v_backgroundColor = a_backgroundColor;
    v_borderRadius = a_borderRadius;
}