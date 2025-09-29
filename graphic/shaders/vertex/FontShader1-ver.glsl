#version 330 core
layout(location=0) in vec2 aPosPx;  // pixel-space position (x,y)
layout(location=1) in vec2 aUV;     // glyph UV
layout(location=2) in vec3 aColor;  // rgb

uniform vec2 u_viewSize;            // framebuffer size in pixels

out vec2 vUV;
out vec3 vColor;

void main() {
    // Convert pixels → NDC, top-left origin
    vec2 ndc;
    ndc.x =  (aPosPx.x / u_viewSize.x) * 2.0 - 1.0;
    ndc.y = -(aPosPx.y / u_viewSize.y) * 2.0 + 1.0;
    gl_Position = vec4(ndc, 0.0, 1.0);

    vUV    = aUV;
    vColor = aColor;
}