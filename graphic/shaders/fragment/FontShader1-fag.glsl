#version 330 core
in vec2 vUV;
in vec3 vColor;
out vec4 FragColor;

uniform sampler2D fontAtlas;

void main() {
    // Your atlas stores in RED channel but swizzles to ALPHA
    // So sample .a (which contains the RED data due to swizzling)
    float coverage = texture(fontAtlas, vUV).a;

    // Simple alpha blending (no SDF)
    FragColor = vec4(vColor * coverage, coverage);

    // Debug: uncomment to see raw coverage values
    // FragColor = vec4(coverage, coverage, coverage, 1.0);
}