#version 330 core
in vec4 v_borderBox;        // xy=min, zw=max (px)
in vec4 v_backgroundColor;  // rgba
in vec4 v_borderRadius;     // (TR, BR, TL, BL)
in vec4 v_shadowProperties; // (offsetX, offsetY, blur, spread)
in vec4 v_shadowColor;      // rgba
in vec4 v_borderWidths;     // (T, R, B, L)  ← NOTE: matches your VS layout
in vec4 v_borderColor;      // rgba
in vec4 v_clippingBox;      // xy=min, zw=max
out vec4 FragColor;

uniform vec2 u_viewSize;

// ---- toggles ---------------------------------------------------------------
#define ENABLE_SHADOW 1   // set to 0 to measure “no shadow” cost

// ---- helpers ---------------------------------------------------------------
float aastep(float edge, float x){
    float w = max(1e-4, fwidth(x));
    return smoothstep(edge - w, edge + w, x);
}

// rectangle signed distance (Chebyshev)
float sdRect(vec2 p, vec2 mn, vec2 mx){
    vec2 d = max(max(mn - p, vec2(0.0)), max(p - mx, vec2(0.0)));
    return max(d.x, d.y);
}

// rounded box SDF (same as yours, but inlined)
float sdRoundedBox(vec2 p, vec2 b, vec4 r){
    vec4 rr = r;
    rr.xy = (p.x > 0.0) ? rr.xy : rr.zw;
    rr.x  = (p.y > 0.0) ? rr.x  : rr.y;
    vec2 q = abs(p) - (b - rr.xx);
    return length(max(q, 0.0)) + min(max(q.x, q.y), 0.0) - rr.x;
}

void premul_over(inout vec4 dst, vec4 src){
    // src assumed straight alpha; convert to premul on the fly
    vec4 s = vec4(src.rgb * src.a, src.a);
    dst.rgb = s.rgb + dst.rgb * (1.0 - s.a);
    dst.a   = s.a   + dst.a   * (1.0 - s.a);
}

void main(){
    // screen-space pixel (Y up)
    vec2 pix = vec2(gl_FragCoord.x, u_viewSize.y - gl_FragCoord.y);

    // clip **before** any work
    if (pix.x <= v_clippingBox.x || pix.x >= v_clippingBox.z ||
    pix.y <= v_clippingBox.y || pix.y >= v_clippingBox.w) discard;

    vec2 mnO = v_borderBox.xy;
    vec2 mxO = v_borderBox.zw;
    vec2 cenO = 0.5*(mnO+mxO);
    vec2 halfO= 0.5*(mxO-mnO);

    // quick flags
    bool hasRadius = (v_borderRadius.x+v_borderRadius.y+v_borderRadius.z+v_borderRadius.w) > 1e-6;
    vec4 bwTRBL = v_borderWidths;          // (T,R,B,L) from VS
    float L = bwTRBL.w, T = bwTRBL.x, R = bwTRBL.y, B = bwTRBL.z;
    bool hasBorder = (L+T+R+B) > 1e-6;

    // ----------------------- OUTER MASK (bg + border) -----------------------
    float aOuter;
    if (!hasRadius){
        // cheap rect SDF
        float dO = sdRect(pix, mnO, mxO);
        aOuter = 1.0 - aastep(0.0, dO);
    } else {
        float dO = sdRoundedBox(pix - cenO, halfO, v_borderRadius);
        aOuter = 1.0 - aastep(0.0, dO);
    }

    // ----------------------- INNER MASK (content) ---------------------------
    float aInner = 0.0;
    if (hasBorder){
        if (!hasRadius){
            // exact per-side shrink for rect
            vec2 mnI = vec2(mnO.x + L, mnO.y + T);
            vec2 mxI = vec2(mxO.x - R, mxO.y - B);
            // guard against inverted boxes
            mnI = min(mnI, mxI - vec2(1e-6));
            float dI = sdRect(pix, mnI, mxI);
            aInner = 1.0 - aastep(0.0, dI);
        } else {
            // approximate inner for rounded: uniform shrink by max width
            float bw = max(max(L,R), max(T,B));
            vec2 halfI = max(halfO - vec2(bw), vec2(0.0));
            vec4 radI  = max(v_borderRadius - vec4(bw), vec4(0.0));
            float dI   = sdRoundedBox(pix - cenO, halfI, radI);
            aInner = 1.0 - aastep(0.0, dI);
        }
    } else {
        aInner = aOuter; // no border → content == outer
    }

    float aBorder = clamp(aOuter - aInner, 0.0, 1.0);

    // ----------------------- PAINT ------------------------------------------
    vec4 outCol = vec4(0.0);

    // background (content)
    if (aInner > 1e-5){
        vec4 bg = v_backgroundColor;
        bg.a *= aInner;
        premul_over(outCol, bg);
    }

    // border
    if (hasBorder && aBorder > 1e-5){
        vec4 bc = v_borderColor;
        bc.a *= aBorder;
        premul_over(outCol, bc);
    }

    // ----------------------- SHADOW (cheap rectangular) ---------------------
    #if ENABLE_SHADOW
    // If blur<=0 or alpha=0 → essentially free
    float blur   = max(v_shadowProperties.z, 0.0);
    float spread = v_shadowProperties.w * 2.0;      // keep same scaling as your src
    vec2  ofs    = v_shadowProperties.xy;

    if (blur > 0.0 && v_shadowColor.a > 1e-5){
        // rectangular shadow around (borderBox expanded by spread) then offset
        vec2 shMn = mnO + min(ofs, vec2(0.0)) - vec2(spread);
        vec2 shMx = mxO + max(ofs, vec2(0.0)) + vec2(spread);

        // rectangular SDF (cheap)
        float dSh = sdRect(pix, shMn, shMx);

        // soft edge: one smoothstep using blur (~penumbra)
        float aSh = 1.0 - smoothstep(0.0, blur + max(1e-4, fwidth(dSh)), dSh);

        // exclude inside the outer silhouette (no shadow inside the shape)
        aSh *= (1.0 - aOuter);

        if (aSh > 1e-5){
            vec4 sh = v_shadowColor;
            sh.a *= aSh;
            premul_over(outCol, sh);
        }
    }
    #endif

    if (outCol.a <= 1e-5) discard;
    FragColor = outCol;
}