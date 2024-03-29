#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
};

const int NR_LIGHTS = 1;
//const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 cameraPosition;

void main()
{             
    // retrieve data from gbuffer
    vec3 FragPos    = texture(gPosition, TexCoords).rgb;
    vec3 Normal     = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse    = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular  = texture(gAlbedoSpec, TexCoords).a;
    
    // then calculate lighting as usual
    vec3 ambient    = vec3(0.2f, 0.2f, 0.2f) * texture(gAlbedoSpec, TexCoords).rgb;
    vec3 lighting   = Diffuse * ambient; // hard-coded ambient component
    vec3 viewDir    = normalize(cameraPosition - FragPos);

    float lightConstantTerm = 1.0;
    float shininess = 16.0;

    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // diffuse
        vec3 lightDir   = normalize(lights[i].Position - FragPos);
        vec3 diffuse    = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;

        // specular
        //vec3 halfwayDir = normalize(lightDir + viewDir);
        vec3 reflectDir = reflect(-lightDir, Normal);
        float spec      = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular   = lights[i].Color * spec * Specular;

        // attenuation
        float distance      = length(lights[i].Position - FragPos);
        float attenuation   = 1.0 / (lightConstantTerm + lights[i].Linear * distance + lights[i].Quadratic * (distance * distance));
        
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
        lighting += ambient + diffuse + specular;       
    }

    FragColor = vec4(lighting, 1.0);
}