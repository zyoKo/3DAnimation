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
    float Constant;
    float Linear;
    float Quadratic;
};

const int NR_LIGHTS = 3;
//const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 cameraPosition;

void main()
{
    // retrieve data from gbuffer
    vec3 FragPos    = texture(gPosition,    TexCoords).rgb;
    vec3 Normal     = texture(gNormal,      TexCoords).rgb;
    vec3 Diffuse    = texture(gAlbedoSpec,  TexCoords).rgb;
    float Specular  = texture(gAlbedoSpec,  TexCoords).a;
    Normal = normalize(Normal);
    
    // hard-coded constants
    float ambientIntensity  = 0.1;
    float shininess         = 16.0;

    vec3 ambient  = vec3(0.0, 0.0, 0.0);
    vec3 diffuse  = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec3 lighting = vec3(0.0, 0.0, 0.0);

    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        vec3 lightDir   = normalize(FragPos - lights[i].Position);
        lightDir        = normalize(lightDir);

        // ambient
        ambient = lights[i].Color * ambientIntensity;

        //-- diffuse
        float diffuseFactor = dot(Normal, -lightDir);
        if (diffuseFactor > 0.0)
        {
            diffuse = diffuseFactor * Diffuse * lights[i].Color;

            vec3 viewDir    = normalize(cameraPosition - FragPos);
            vec3 reflectDir = normalize(reflect(lightDir, Normal));

            //-- specular
            float specularFactor = dot(viewDir, reflectDir);
            if (specularFactor > 0.0)
            {
                specularFactor  = pow(specularFactor, shininess);
                specular        = lights[i].Color * specularFactor * Specular;
            }
        }

        // attenuation
        float distance          = length(lightDir);
        float attenuationFactor = lights[i].Constant + lights[i].Linear * distance + lights[i].Quadratic * (distance * distance);
        float attenuation       = 1.0 / max(1.0, attenuationFactor);
        
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
        lighting  = ambient + diffuse + specular;       
    }

    FragColor = vec4(lighting, 1.0);
}