#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 FragPos;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

//out vec4 FragColor;

void main()
{
    gPosition = FragPos;

    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    gAlbedoSpec.a = 1.0f;
    
    //FragColor =  texture(texture_diffuse1, TexCoords);
}