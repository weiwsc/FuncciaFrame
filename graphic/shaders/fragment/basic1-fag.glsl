#version 410 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

in vec3 facing;
// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    vec3 camera = vec3(-0.3,-0.5,1);
    float normal = dot(camera, facing);
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 1);
}