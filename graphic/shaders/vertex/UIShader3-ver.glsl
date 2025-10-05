#version 330 core
layout(location=0) in vec2 a_unit;          // 0..1 quad
layout(location=1) in vec4 a_borderBox;     // xy=min(px), zw=max(px)
layout(location=2) in vec4 a_bg;
layout(location=3) in vec4 a_radius;
layout(location=4) in vec4 a_shadowProps;   // ofsX, ofsY, blur, spread
layout(location=5) in vec4 a_shadowCol;
layout(location=6) in vec4 a_borderWidths;  // (T, R, B, L)  (matches your FS)
layout(location=7) in vec4 a_borderColor;
layout(location=8) in vec4 a_clippingBox;   // xy=min(px), zw=max(px)

uniform vec2 u_viewSize;

// varyings (unchanged so your FS still works)
out vec4 v_borderBox;
out vec4 v_backgroundColor;
out vec4 v_borderRadius;
out vec4 v_shadowProperties;
out vec4 v_shadowColor;
out vec4 v_borderWidths;
out vec4 v_borderColor;
out vec4 v_clippingBox; // kept for A/B; you can delete in FS once happy

void main() {
    // ---- build the expanded cover quad (for shadow/blur) ----
    vec2 ofs     = a_shadowProps.xy;
    float blur   = max(a_shadowProps.z, 0.0);
    float spread = a_shadowProps.w;

    float maxBorder = max(max(a_borderWidths.x, a_borderWidths.y),
    max(a_borderWidths.z, a_borderWidths.w));

    vec2 pad      = vec2(spread + blur + maxBorder);
    vec2 coverMin = a_borderBox.xy + min(ofs, vec2(0.0)) - pad;
    vec2 coverMax = a_borderBox.zw + max(ofs, vec2(0.0)) + pad;

    vec2 posPx = mix(coverMin, coverMax, a_unit);

    // ---- position in NDC ----
    vec2 ndc;
    ndc.x =  (posPx.x / u_viewSize.x) * 2.0 - 1.0;
    ndc.y = -(posPx.y / u_viewSize.y) * 2.0 + 1.0;
    gl_Position = vec4(ndc, 0.0, 1.0);

    // ---- hardware clip using gl_ClipDistance (axis-aligned rect) ----
    // convert pixel clip rect to NDC. Note the Y flip to match your coords.
    vec4 clipPix = a_clippingBox; // (minX, minY, maxX, maxY) in pixels
    vec4 clipNDC = vec4(
    (clipPix.x / u_viewSize.x) * 2.0 - 1.0,      // left
    -(clipPix.w / u_viewSize.y) * 2.0 + 1.0,      // top
    (clipPix.z / u_viewSize.x) * 2.0 - 1.0,      // right
    -(clipPix.y / u_viewSize.y) * 2.0 + 1.0       // bottom
    );

    // Inside region is where these are all >= 0
    gl_ClipDistance[0] = ndc.x - clipNDC.x; // left plane
    gl_ClipDistance[1] = clipNDC.z - ndc.x; // right
    gl_ClipDistance[2] = ndc.y - clipNDC.y; // top
    gl_ClipDistance[3] = clipNDC.w - ndc.y; // bottom

    // ---- pass-throughs for FS ----
    v_borderBox        = a_borderBox;
    v_backgroundColor  = a_bg;
    v_borderRadius     = a_radius;
    v_shadowProperties = a_shadowProps;
    v_shadowColor      = a_shadowCol;
    v_borderWidths     = a_borderWidths;
    v_borderColor      = a_borderColor;
    v_clippingBox      = a_clippingBox; // keep for now (FS can still use/discard)
}