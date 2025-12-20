#version 330 core
in vec2 vUV;
in vec4 vColor;
flat in float vLayer;

uniform sampler2DArray uAtlas;   // <-- array texture
uniform float uSDFPxRange;

out vec4 frag;

void main() {
    float sd = texture(uAtlas, vec3(vUV, vLayer)).r;

    // Better SDF rendering with configurable sharpness
    float screenPxDistance = uSDFPxRange * (sd - 0.5);
    float screenPxRange = 0.5 * length(vec2(dFdx(screenPxDistance), dFdy(screenPxDistance)));
    float alpha = smoothstep(-screenPxRange, screenPxRange, screenPxDistance);

    frag = vec4(vColor.rgb, vColor.a * alpha);
}