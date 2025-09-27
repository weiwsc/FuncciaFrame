#version 330 core

in vec2 v_pixelPos;
in vec4 v_borderBox;
in vec4 v_backgroundColor;
in vec4 v_borderRadius;

out vec4 FragColor;

float sdRoundedBox( in vec2 p, in vec2 b, in vec4 r ) {
    r.xy = (p.x>0.0)?r.xy : r.zw;
    r.x = (p.y>0.0)?r.x : r.y;
    vec2 q = abs(p)-b+r.x;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r.x;
}

void main() {
    vec2 size = (v_borderBox.zw - v_borderBox.xy) * 0.5;
    vec2 center = (v_borderBox.xy + v_borderBox.zw) * 0.5;

    float dist = sdRoundedBox(v_pixelPos - center, size, v_borderRadius);

    float fw = fwidth(dist);
    float alpha = 1.0 - smoothstep(0.0, fw, dist);

    vec4 color = v_backgroundColor;
    color.a *= alpha;

    FragColor = color;
}