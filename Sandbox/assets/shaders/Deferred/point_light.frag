#version 460 core

out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light
{
    vec3 Position;
    vec3 Color;

    float Radius;

    float Constant;
    float Linear;
    float Quadratic;

    float AmbientIntensity;
    float Intensity;
};

uniform Light light;
uniform vec3 cameraPosition;
uniform vec2 screenSize;

void main()
{
    vec2 texCoord = gl_FragCoord.xy / screenSize;

    // retrieve data from gbuffer
    vec3 FragPos    = texture(gPosition,    texCoord).rgb;
    vec3 Normal     = texture(gNormal,      texCoord).rgb;
    vec3 Diffuse    = texture(gAlbedoSpec,  texCoord).rgb;
    float Specular  = texture(gAlbedoSpec,  texCoord).a;
    Normal = normalize(Normal);
    
    // hard-coded constants
    float shininess        = 16.0;

    // then calculate lighting as usual
    vec3 lighting = vec3(0.0, 0.0, 0.0);
    vec3 ambient  = vec3(0.0, 0.0, 0.0);
    vec3 diffuse  = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);

    vec3 lightDir = normalize(light.Position - FragPos);
    vec3 viewDir  = normalize(cameraPosition - FragPos);

    // calculate distance between light source and current fragment
    float distance = length(light.Position - FragPos);
    if(distance < light.Radius)
    {
        // this is need to fade out light intensity before we reach the edges on the sphere
        float attenuationSmoothness = 1.0 - smoothstep(0.0, light.Radius, distance);

        // ambient
        ambient  = light.Color * light.AmbientIntensity;

        lighting = (Diffuse * ambient) * attenuationSmoothness;

        // diffuse
        float diffuseFactor = dot(Normal, lightDir);
        diffuse  = (max(diffuseFactor, 0.0) * Diffuse * light.Color) * attenuationSmoothness;

        // specular
        vec3 halfwayDir         = normalize(lightDir + viewDir);
        float specularFactor    = dot(Normal, halfwayDir);
        float spec              = pow(max(specularFactor, 0.0), shininess);
        specular = (light.Color * spec * Specular) * attenuationSmoothness;
    }

    // attenuation
    float attenuationFactor = light.Constant + (light.Linear * distance) + (light.Quadratic * (distance * distance));
    float attenuation       = (1.0 / max(1.0, attenuationFactor));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    lighting += ambient + (diffuse + specular) * light.Intensity;

    FragColor = vec4(lighting, 1.0);
}