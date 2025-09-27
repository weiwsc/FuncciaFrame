#version 410 core
in vec2 vUV;
in vec3 vColor;
out vec4 FragColor;

uniform sampler2D fontAtlas;

void main(){
    float distance = texture(fontAtlas, vUV).a;

    // This is the key: fwidth() automatically adjusts smoothing based on zoom level
    float width = fwidth(distance);
    float alpha = smoothstep(0.5 - width, 0.5 + width, distance);

    FragColor = vec4(vColor, alpha);
}