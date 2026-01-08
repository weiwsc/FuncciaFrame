#version 330 core
layout(location=0) in vec2 aUnit;     // (0,0),(1,0),(1,1),(0,1)
layout(location=1) in vec2 iOrigin;   // already includes HB offsets
layout(location=2) in vec2 iSize;
layout(location=3) in vec2 iBearing;  // (left, top) — top is y-up
layout(location=4) in vec4 iUVRect;
layout(location=5) in vec4 iColor;
layout(location=6) in float iLayer;
layout(location=7) in vec4 a_clippingBox;
uniform vec2 u_viewSize;

out vec2 vUV;
out vec4 vColor;
out vec4 v_clippingBox;
flat out float vLayer;

void main() {
    vec2 topLeft = vec2(iOrigin.x + iBearing.x,
    iOrigin.y - iBearing.y);  // y-down

    vec2 posPx = topLeft + aUnit * iSize;

    vec2 ndc;
    ndc.x =  (posPx.x / u_viewSize.x) * 2.0 - 1.0;
    ndc.y = -(posPx.y / u_viewSize.y) * 2.0 + 1.0;

    gl_Position = vec4(ndc, 0.0, 1.0);

    vUV    = mix(iUVRect.xy, iUVRect.zw, aUnit);
    vColor = iColor;
    vLayer = iLayer;
    v_clippingBox = a_clippingBox;
}