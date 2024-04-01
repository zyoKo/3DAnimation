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
    float ambientIntensity      = 0.1;
    float lightColorIntensity   = 5.0;
    float shininess             = 16.0;

    // then calculate lighting as usual
    vec3 lighting = vec3(0.0, 0.0, 0.0);
    vec3 ambient  = vec3(0.0, 0.0, 0.0);
    vec3 diffuse  = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);

    vec3 viewDir  = normalize(cameraPosition - FragPos);

    // calculate distance between light source and current fragment
    float distance = length(light.Position - FragPos);
    if(distance < light.Radius)
    {
        // ambient
        ambient  = light.Color * ambientIntensity;  // hard-coded ambient intensity

        lighting = Diffuse * ambient;

        // diffuse
        vec3 lightDir       = normalize(light.Position - FragPos);
        float diffuseFactor = dot(Normal, lightDir);
        diffuse             = max(diffuseFactor, 0.0) * Diffuse * light.Color;

        // specular
        vec3 halfwayDir         = normalize(lightDir + viewDir);
        float specularFactor    = dot(Normal, halfwayDir);
        float spec              = pow(max(specularFactor, 0.0), shininess);
        specular                = light.Color * spec * Specular;
    }

    // attenuation
    float attenuationSmoothness = smoothstep(0.0, light.Radius, distance);
    float attenuationFactor = light.Constant + (light.Linear * distance) + (light.Quadratic * (distance * distance));
    float attenuation       = (1.0 / max(1.0, attenuationFactor)) * attenuationSmoothness;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    lighting += ambient + (diffuse + specular) * lightColorIntensity;

    FragColor = vec4(lighting, 1.0);
}