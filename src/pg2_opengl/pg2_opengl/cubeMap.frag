#version 330 core
out vec4 FragColor;
in vec3 position_ws;

uniform samplerCube cubeMap;

void main()
{		
    vec3 color = texture(cubeMap, position_ws).rgb;
    
    // HDR tonemap and gamma correct
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); 
    
    FragColor = vec4(color, 1.0);
}