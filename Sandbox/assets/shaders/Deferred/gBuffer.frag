#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

// For SkySphere
in vec4 s_Pos;
uniform bool isSkySphere;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

const float PI = 3.14159;

void main()
{
    gPosition       = FragPos;  // Does it contribute in IBL?
    gNormal = normalize(Normal);
    
    if (isSkySphere)
    {
        vec2 uv = vec2(0.5f - (atan(s_Pos.z, s_Pos.x) / (2 * PI)), acos(s_Pos.y) / PI);
        vec3 diffuse = texture(texture_diffuse1, uv).rgb;
        diffuse = pow(diffuse, vec3(2.2));  // this is making the skySphere look dull

        gAlbedoSpec = vec4(diffuse, 1.0);
        return;
    }

    gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
    gAlbedoSpec.a   = texture(texture_specular1, TexCoords).r;
}