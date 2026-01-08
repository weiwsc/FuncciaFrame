#version 330 core
in vec2 vUV;
in vec4 vColor;
in vec4 v_clippingBox;
flat in float vLayer;

uniform sampler2DArray uAtlas;   // <-- array texture
uniform float uSDFPxRange;
uniform vec2 u_viewSize;

out vec4 frag;

void main() {
    vec2 pix = vec2(gl_FragCoord.x, u_viewSize.y - gl_FragCoord.y);
    if (pix.x <= v_clippingBox.x || pix.x >= v_clippingBox.z ||
    pix.y <= v_clippingBox.y || pix.y >= v_clippingBox.w) {
        discard;
    }
    float sd = texture(uAtlas, vec3(vUV, vLayer)).r;

    // Better SDF rendering with configurable sharpness
    float screenPxDistance = uSDFPxRange * (sd - 0.5);
    float screenPxRange = 0.5 * length(vec2(dFdx(screenPxDistance), dFdy(screenPxDistance)));
    float alpha = smoothstep(-screenPxRange, screenPxRange, screenPxDistance);

    frag = vec4(vColor.rgb, vColor.a * alpha);
}