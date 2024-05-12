#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light
{
    vec3 Position;
    vec3 Color;
};

uniform Light light;
uniform vec3 cameraPosition;

void main()
{
    // retrieve data from gbuffer
    vec3 FragPos    = texture(gPosition,    TexCoords).rgb;
    vec3 Normal     = texture(gNormal,      TexCoords).rgb;
    vec3 Diffuse    = texture(gAlbedoSpec,  TexCoords).rgb;
    float Specular  = texture(gAlbedoSpec,  TexCoords).a;
    
    // then calculate lighting as usual
    vec3 ambient    = vec3(0.5f, 0.5f, 0.5f) * Diffuse;
    vec3 viewDir    = normalize(cameraPosition - FragPos);

    float shininess = 16.0;

    // diffuse
    vec3 lightDir   = normalize(light.Position - FragPos);
    vec3 diffuse    = max(dot(Normal, lightDir), 0.0) * Diffuse * light.Color;
    diffuse /= 10.0;

    // specular
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular   = light.Color * spec * Specular;

    vec3 color = ambient + diffuse + specular;

    FragColor = vec4(color, 1.0);
    //FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}