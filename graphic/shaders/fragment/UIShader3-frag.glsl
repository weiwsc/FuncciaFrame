#version 330 core
in vec4 v_borderBox;
in vec4 v_backgroundColor;
in vec4 v_borderRadius;
in vec4 v_shadowProperties;
in vec4 v_shadowColor;
in vec4 v_borderWidths;
in vec4 v_borderColor;
in vec4 v_clippingBox;
out vec4 FragColor;

uniform vec2 u_viewSize;

// Inner shadow controls (macOS-style)
const float INNER_SHADOW_SIZE = 6.0;
const float INNER_SHADOW_ALPHA = 0.07;
const float TOP_HIGHLIGHT_ALPHA = 0.09;
const float TOP_BIAS = 0.9;
const vec3 INNER_SHADOW_TINT = vec3(0.0);
const vec3 TOP_HIGHLIGHT_TINT = vec3(1.0, 0.99, 0.97);

float sdRoundedBox(vec2 p, vec2 b, vec4 r) {
    vec4 rr = r;
    rr.xy = (p.x > 0.0) ? rr.xy : rr.zw;
    rr.x  = (p.y > 0.0) ? rr.x  : rr.y;
    vec2 q = abs(p) - (b - rr.xx);
    return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - rr.x;
}

float aastep(float edge, float x) {
    float w = max(1e-4, fwidth(x));
    return smoothstep(edge - w, edge + w, x);
}

void premul_over(inout vec4 dst, vec4 src) {
    vec4 s = vec4(src.rgb * src.a, src.a);
    dst.rgb = s.rgb + dst.rgb * (1.0 - s.a);
    dst.a   = s.a   + dst.a   * (1.0 - s.a);
}

void main() {
    vec2 pix = vec2(gl_FragCoord.x, u_viewSize.y - gl_FragCoord.y);

    // Early discard for clipping
    if (pix.x <= v_clippingBox.x || pix.x >= v_clippingBox.z ||
    pix.y <= v_clippingBox.y || pix.y >= v_clippingBox.w) {
        discard;
    }

    // Outer box
    vec2 outerMin = v_borderBox.xy;
    vec2 outerMax = v_borderBox.zw;
    vec2 halfO = 0.5 * (outerMax - outerMin);
    vec2 centerO = 0.5 * (outerMax + outerMin);

    float dOuter = sdRoundedBox(pix - centerO, halfO, v_borderRadius);
    float aOuter = 1.0 - aastep(0.0, dOuter);

    // Inner box (content area)
    float L = v_borderWidths.w;
    float T = v_borderWidths.x;
    float R = v_borderWidths.y;
    float B = v_borderWidths.z;

    vec2 halfI = max(halfO - 0.5 * vec2(L + R, T + B), vec2(0.0));
    vec2 centerI = centerO + 0.5 * vec2(L - R, B - T);

    float rTR = max(v_borderRadius.x - max(R, T), 0.0);
    float rBR = max(v_borderRadius.y - max(R, B), 0.0);
    float rTL = max(v_borderRadius.z - max(L, T), 0.0);
    float rBL = max(v_borderRadius.w - max(L, B), 0.0);
    vec4 radI = vec4(rTR, rBR, rTL, rBL);

    float dInner = sdRoundedBox(pix - centerI, halfI, radI);
    float aInner = 1.0 - aastep(0.0, dInner);
    float aBorder = clamp(aOuter - aInner, 0.0, 1.0);

    // === DROP SHADOW (Optimized macOS-style) ===
    vec4 outCol = vec4(0.0);

    float blur = v_shadowProperties.z;
    if (blur > 0.0 && v_shadowColor.a > 0.01) {
        vec2 offset = v_shadowProperties.xy;
        float spread = v_shadowProperties.w * 2.0;

        vec2 shCenter = centerO + offset;
        vec2 shHalf = halfO + vec2(spread);
        vec4 shR = clamp(v_borderRadius + vec4(spread), 0.0, min(shHalf.x, shHalf.y));

        float dSh = sdRoundedBox(pix - shCenter, shHalf, shR);

        // Optimized blur: single smoothstep instead of complex penumbra
        // macOS uses a softer, wider falloff
        float shadowFalloff = 1.0 - smoothstep(-blur * 0.5, blur * 1.5, dSh);
        shadowFalloff *= (1.0 - aOuter); // Only outside the element

        if (shadowFalloff > 0.01) {
            vec4 shadow = v_shadowColor;
            shadow.a *= shadowFalloff;
            premul_over(outCol, shadow);
        }
    }

    // === BACKGROUND ===
    if (aInner > 1e-5) {
        vec4 bg = v_backgroundColor;
        bg.a *= aInner;
        premul_over(outCol, bg);
    }

    // === BORDER ===
    if (aBorder > 1e-5 && (L + T + R + B) > 0.0) {
        vec4 bc = v_borderColor;
        bc.a *= aBorder;
        premul_over(outCol, bc);
    }

    // === INNER SHADOW (macOS-style soft inset) ===
    if (aInner > 1e-5) {
        // Distance-based falloff from edges
        float edgeProximity = 1.0 - smoothstep(-INNER_SHADOW_SIZE, 0.0, dInner);

        // Base inner shadow (uniform around all edges)
        float innerAlpha = edgeProximity * INNER_SHADOW_ALPHA;
        vec4 innerShade = vec4(INNER_SHADOW_TINT, innerAlpha) * aInner;
        premul_over(outCol, innerShade);

        // Top highlight (macOS characteristic)
        float contentTop = outerMin.y + T;
        float contentBottom = outerMax.y - B;

        // Smooth gradient from top, weighted by edge proximity
        float topDist = pix.y - contentTop;
        float topRamp = 1.0 - smoothstep(0.0, INNER_SHADOW_SIZE, topDist);
        topRamp = pow(topRamp, mix(1.0, 2.2, TOP_BIAS));

        float highlightAlpha = topRamp * edgeProximity * TOP_HIGHLIGHT_ALPHA;
        vec4 topHighlight = vec4(TOP_HIGHLIGHT_TINT, highlightAlpha) * aInner;
        premul_over(outCol, topHighlight);
    }

    if (outCol.a <= 1e-5) discard;
    FragColor = outCol;
}