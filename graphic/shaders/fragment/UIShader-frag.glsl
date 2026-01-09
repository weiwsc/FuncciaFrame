// Fragment Shader – crisp per-side borders + consistent AA (same inputs)
#version 330 core
in vec4 v_borderBox;        // xy = min, zw = max (screen space)
in vec4 v_backgroundColor;  // rgba
in vec4 v_borderRadius;     // (TR, BR, TL, BL)  ← matches the original sdRoundedBox usage
in vec4 v_shadowProperties; // (offsetX, offsetY, blur, spread)
in vec4 v_shadowColor;      // rgba
in vec4 v_borderWidths;     // (L, T, R, B)
in vec4 v_borderColor;      // rgba
in vec4 v_clippingBox;
out vec4 FragColor;

uniform vec2 u_viewSize;

// --- Utilities --------------------------------------------------------------

float sdRoundedBox(vec2 p, vec2 b, vec4 r){
    // p: point in box-local space (centered), b: half-size, r: (TR, BR, TL, BL)
    vec4 rr = r;
    rr.xy = (p.x > 0.0) ? rr.xy : rr.zw; // choose (TR,BR) or (TL,BL) by x
    rr.x  = (p.y > 0.0) ? rr.x  : rr.y;  // then choose top vs bottom by y
    vec2 q = abs(p) - (b - rr.xx);
    return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - rr.x;
}

float aastep(float edge, float x){
    float w = max(1e-4, fwidth(x));
    return smoothstep(edge - w, edge + w, x);
}

float fillAlpha(float d){
    // 1 inside the shape, smooth edge around d=0
    return 1.0 - aastep(0.0, d);
}

vec4 renderShadow(vec2 pix, vec2 center, vec2 halfSize, vec4 radius,
vec2 offset, float blur, float spread,
vec4 shadowColor, float excludeMask)
{
    if (shadowColor.a <= 0.0 || blur <= 0.0) return vec4(0.0);

    vec2 shCenter = center + offset;
    vec2 shHalf   = halfSize + vec2(spread);
    vec4 shR      = clamp(radius + vec4(spread), 0.0, min(shHalf.x, shHalf.y));

    // Multi-sample for smoother corners
    float alpha = 0.0;
    float sigma = blur * 0.5;

    // 9-tap grid sample
    for (float dx = -1.0; dx <= 1.0; dx += 1.0) {
        for (float dy = -1.0; dy <= 1.0; dy += 1.0) {
            vec2 samplePos = pix + vec2(dx, dy) * sigma * 0.5;
            float d = sdRoundedBox(samplePos - shCenter, shHalf, shR);

            // Gaussian weight based on distance
            float g = exp(-(d * d) / (2.0 * sigma * sigma));
            alpha += d < 0.0 ? 1.0 : g;
        }
    }
    alpha /= 9.0;

    alpha *= excludeMask;
    if (alpha <= 1e-5) return vec4(0.0);

    vec4 sh = shadowColor;
    sh.a *= alpha;
    return sh;
}

// --- Main -------------------------------------------------------------------

void main(){
    // Screen-space pixel (Y flipped to match your box coords)
    vec2 pix    = vec2(gl_FragCoord.x, u_viewSize.y - gl_FragCoord.y);

    if (pix.x <= v_clippingBox.x || pix.x >= v_clippingBox.z ||
    pix.y <= v_clippingBox.y || pix.y >= v_clippingBox.w) {
        discard;
    }
    // Outer (border box)
    vec2 outerMin = v_borderBox.xy;
    vec2 outerMax = v_borderBox.zw;
    vec2 halfO    = 0.5 * (outerMax - outerMin);
    vec2 centerO  = 0.5 * (outerMax + outerMin);

    // Signed distance for the outer rounded rect
    float dOuter  = sdRoundedBox(pix - centerO, halfO, v_borderRadius);
    float aOuter  = fillAlpha(dOuter);   // 1 inside outer shape (border+content)

    // --- Inner (content box) computed per-side ------------------------------
    // v_borderWidths = (L,T,R,B)
    float L = v_borderWidths.w;
    float T = v_borderWidths.x;
    float R = v_borderWidths.y;
    float B = v_borderWidths.z;

    // Inner edges (move each side inward); this also implies a center shift
    vec2 halfI   = max(halfO - 0.5 * vec2(L + R, T + B), vec2(0.0));
    vec2 centerI = centerO + 0.5 * vec2(L - R, B - T); // note Y: top shrinks upward

    // Corner radii shrink by adjacent side thickness (CSS-like)
    // v_borderRadius order matches sdRoundedBox selection: (TR, BR, TL, BL)
    float rTR = max(v_borderRadius.x - max(R, T), 0.0);
    float rBR = max(v_borderRadius.y - max(R, B), 0.0);
    float rTL = max(v_borderRadius.z - max(L, T), 0.0);
    float rBL = max(v_borderRadius.w - max(L, B), 0.0);
    vec4  radI = vec4(rTR, rBR, rTL, rBL);

    float dInner = sdRoundedBox(pix - centerI, halfI, radI);
    float aInner = fillAlpha(dInner);    // 1 inside content (no border)

    // Masks
    float aContent = aInner;
    float aBorder  = clamp(aOuter - aInner, 0.0, 1.0); // crisp ring (non-overlapping)

    // --- Background fill (content) ------------------------------------------
    vec4 col = vec4(0.0);
    if (aContent > 1e-5){
        vec4 bg = v_backgroundColor;
        bg.a *= aContent;
        col.rgb += bg.rgb * bg.a;
        col.a   += bg.a * (1.0 - col.a);
    }

    // --- Border paint (clean, even thickness on all sides) ------------------
    if (aBorder > 1e-5 && (L+T+R+B) > 0.0){
        vec4 bc = v_borderColor;
        bc.a *= aBorder;
        col.rgb += bc.rgb * bc.a * (1.0 - col.a);
        col.a   += bc.a * (1.0 - col.a);
    }

    // --- Shadow under the shape (excluded by the outer silhouette) ----------
    vec4 dropShadow = renderShadow(
    pix, centerO, halfO, v_borderRadius,
    v_shadowProperties.xy,
    v_shadowProperties.z,
    v_shadowProperties.w * 2.0,
    v_shadowColor,
    1.0 - aOuter // only outside the silhouette
    );

    if (dropShadow.a > 1e-5){
        // Premultiplied "under" blend
        vec3 premSh = dropShadow.rgb * dropShadow.a;
        vec3 premBk = col.rgb * col.a;
        col.rgb = premSh + premBk * (1.0 - dropShadow.a);
        col.a   = dropShadow.a + col.a * (1.0 - dropShadow.a);
    }

    if (col.a <= 1e-5) discard;
    FragColor = col;
}