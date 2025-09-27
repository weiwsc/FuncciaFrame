#version 410 core
out vec4 FragColor;
in vec3 worldPos;

void main()
{
    vec3 cameraPos    = vec3(0.0, 0.0, 5.0);

    vec3 lightPos = vec3(-10.0, 15.0, 18.0);  // Fixed in world space
    vec3 objectColor = vec3(0.35,0.4,1);
    vec3 ambientColor = vec3(0.8, 0.2, 0.2);
    vec3 lightColor = vec3(0.2,0.6,0.7);

    vec3 normal = normalize(cross(dFdx(worldPos), dFdy(worldPos)));
    vec3 lightDir = normalize(lightPos - worldPos);
    float light = max(dot(normal, lightDir), 0.0);

    //FragColor = vec4(objectColor * (0.2 + 0.8 * light), 1.0);
    vec4 FragColor1 = mix(vec4(objectColor,1), vec4(lightColor,1), light*0.8 + 0.6);
    vec4 FragColor2 = mix(vec4(ambientColor,1), vec4(objectColor,1), light);
    FragColor2 = mix(FragColor1, FragColor2, light*0.3 + 0.7);

    vec4 baseColor = mix(FragColor2, FragColor1, light);


    vec3 edgeColor    = vec3(1, 0.3, 0.5);


    vec3 viewDir = normalize(cameraPos - worldPos);
    float facing = max(dot(normal, viewDir), 0.0);
    float edge   = pow(1.0 - facing, 2.0); // 0 in center, 1 on edges

    vec3 finalCol = mix(baseColor.rgb, edgeColor, edge * 0.5);

    FragColor = vec4(finalCol, 1.0);
}