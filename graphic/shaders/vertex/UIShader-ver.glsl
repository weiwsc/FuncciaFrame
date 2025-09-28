// Vertex Shader - Same as your working version with border attributes
#version 330 core
layout(location=0) in vec2 a_unit;          // 0..1 quad verts (2 tris)
layout(location=1) in vec4 a_borderBox;     // xy=min(px), zw=max(px)
layout(location=2) in vec4 a_bg;
layout(location=3) in vec4 a_radius;
layout(location=4) in vec4 a_shadowProps;   // ofsX, ofsY, blur, spread
layout(location=5) in vec4 a_shadowCol;
layout(location=6) in vec4 a_borderWidths;  // top, right, bottom, left
layout(location=7) in vec4 a_borderColor;

uniform vec2 u_viewSize;

out vec4 v_borderBox;
out vec4 v_backgroundColor;
out vec4 v_borderRadius;
out vec4 v_shadowProperties;
out vec4 v_shadowColor;
out vec4 v_borderWidths;
out vec4 v_borderColor;

void main() {
    vec2 ofs    = a_shadowProps.xy;
    float blur  = max(a_shadowProps.z, 0.0);
    float spread= a_shadowProps.w;

    // Include border in coverage
    float maxBorder = max(max(a_borderWidths.x, a_borderWidths.y),
    max(a_borderWidths.z, a_borderWidths.w));

    vec2 pad = vec2(spread + blur + maxBorder);

    vec2 coverMin = a_borderBox.xy + min(ofs, vec2(0.0)) - pad;
    vec2 coverMax = a_borderBox.zw + max(ofs, vec2(0.0)) + pad;

    vec2 posPx = mix(coverMin, coverMax, a_unit);

    vec2 ndc;
    ndc.x =  (posPx.x / u_viewSize.x) * 2.0 - 1.0;
    ndc.y = -(posPx.y / u_viewSize.y) * 2.0 + 1.0;

    gl_Position = vec4(ndc, 0.0, 1.0);

    v_borderBox        = a_borderBox;
    v_backgroundColor  = a_bg;
    v_borderRadius     = a_radius;
    v_shadowProperties = a_shadowProps;
    v_shadowColor      = a_shadowCol;
    v_borderWidths     = a_borderWidths;
    v_borderColor      = a_borderColor;
}