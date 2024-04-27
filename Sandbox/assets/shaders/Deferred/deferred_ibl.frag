#version 460 core

// Final output
out vec4 FragColor;
out vec4 BrightColor;

in vec2 TexCoords;

// G-Buffer
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

// IBL
uniform sampler2D skySphere;
uniform sampler2D irradianceMap;

// AO
uniform sampler2D ambientOcclusionMap;

// Directional Light
struct Light
{
    vec3 Direction;
    vec3 Color;
};
uniform Light light;

// Hammersly Block Uniforms
const int HAMMERSLY_BLOCK_SIZE = 2 * 50;
uniform int N_Hammersley; // 50
layout (binding = 0) uniform HammersleyBlock 
{
    float hammersley[HAMMERSLY_BLOCK_SIZE];
} hammersleyBlock;

uniform float exposure;

// Camera Position(View Position)
uniform vec3 cameraPosition;

// Screen Size (buffer is equivalent to screen in this case)
uniform float bufferWidth;
uniform float bufferHeight;

// metallic and roughness constant
uniform float metallic;
uniform float roughness;

// directional light vs IBL
uniform bool useDirectionalLight;
uniform bool usingBloom;
uniform bool useAO;

// Constants
const float PI = 3.14159265359f;

// Function Declerations
float LOG_DistributionGGX(vec3 N, vec3 H, float roughness);
float Herron_DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3  FresnelSchlick(float cosTheta, vec3 F0);
vec3  FresnelSchickRoughness(float cosTheta, vec3 F0, float roughness);
float ClampZeroToOne(float value);
vec2  CalculateUVBasedOnDirection(vec3 direction);

vec3 PhongLighting(vec3 FragPos, vec3 Normal, vec3 Diffuse, float Specular);
vec3 ComputeColorForIBL(vec3 FragPos, vec3 Normal, vec3 Diffuse, vec3 specular);

void main()
{
    // retrieve data from gbuffer
    vec3 FragPos  = texture(gPosition,    TexCoords).rgb;
    vec3 Normal   = texture(gNormal,      TexCoords).rgb;
    vec3 Diffuse  = texture(gAlbedoSpec,  TexCoords).rgb;
    vec3 specular = vec3(0.0, 0.0, 0.0);

    vec3 color = vec3(0.0);
    if (useDirectionalLight)
    {
        color += PhongLighting(FragPos, Normal, Diffuse, 1.0);
    }
    else
    {
        color += ComputeColorForIBL(FragPos, Normal, Diffuse, specular);
    }

    // extract bright colors to compute bloom
    if (usingBloom)
    {
        float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
        if(brightness > 1.0)
        {
            BrightColor = vec4(color, 1.0);
        }
        else
        {
            BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
        }
    }

    if (!useDirectionalLight)
    {
        // apply gamma correction (if using IBL)
        color = color / (color + vec3(1.0));
        color = pow(color, vec3(1.0 / 2.2));
    }

    FragColor = vec4(color, 1.0);
}

// ----------------------------------------------------------------------------
float Herron_DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float dotHN = max(dot(H, N), 0.0);
    float tan_theta_m = sqrt((1.0f - pow(dotHN, 2))) / dotHN;
    float tan_square_theta_m = pow(tan_theta_m, 2);
    float alpha_square = roughness * roughness;

    float denom = PI * pow(dotHN, 4) * pow(alpha_square + tan_square_theta_m, 2);
    return clamp(dotHN, 0.0, 1.0) * (roughness * roughness / denom);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float roughnessSquare = roughness * roughness;
    float roughnessFourth = roughnessSquare * roughnessSquare;

    float NdotH = max(dot(N, H), 0.0);
    float NdotHSquare = NdotH * NdotH;

    // distribution
    float numerator = roughnessFourth;
    float denominator = (NdotHSquare * (roughnessFourth - 1.0) + 1.0);
    denominator = PI * denominator * denominator;

    return (numerator / denominator);
}

// ----------------------------------------------------------------------------
float GeometrySchlickGGX(vec3 N, vec3 V, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float NdotV = max(dot(N, V), 0.0);

    float numerator   = NdotV;
    float denominator = NdotV * (1.0 - k) + k;

    return numerator / denominator;
}

// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float ggx2 = GeometrySchlickGGX(N, V, roughness);
    float ggx1 = GeometrySchlickGGX(N, L, roughness);

    return ggx1 * ggx2;
}

// ----------------------------------------------------------------------------
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// ----------------------------------------------------------------------------
vec3 FresnelSchickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// ----------------------------------------------------------------------------
float ClampZeroToOne(float value)
{
    return clamp(value, 0.0, 1.0);
}

// ----------------------------------------------------------------------------
vec2 CalculateUVBasedOnDirection(vec3 direction)
{
    float u = 0.5 - atan(-direction.z, direction.x) / (2.0 * PI);
    float v = acos(direction.y) / PI;

    return vec2(u, v);
}

// ----------------------------------------------------------------------------
vec3  ComputeColorForIBL(vec3 FragPos, vec3 Normal, vec3 Diffuse, vec3 specular)
{
    // Rewriting for convinience
    vec3 N = normalize(Normal); // should be normalized
    vec3 L = normalize(-light.Direction);
    vec3 V = normalize(cameraPosition - FragPos);
    vec3 H = normalize(V + L);

    float NdotL = max(dot(N, L), 0.0);
    float HdotL = max(dot(H, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float HdotV = max(dot(H, V), 0.0);

    // Calculate reflectance at normal incidence
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Diffuse, metallic);

    // Diffuse Color
    vec3 Ks = FresnelSchickRoughness(NdotV, F0, roughness);
    vec3 Kd = vec3(1.0) - Ks;
    Kd *= 1.0 - metallic;

    // rotation frame is reflection vector R
    vec3 R = 2.0 * NdotV * N - V;
    vec3 A = normalize(vec3(-R.y, R.x, 0.0));
    vec3 B = normalize(cross(R, A));

    // calculate specular from skySphere
    for (int i = 0; i < N_Hammersley * 2; i += 2)
    {
        // Random Numbers from Hammersley Block
        vec2 hammersleyRand = vec2(hammersleyBlock.hammersley[i], hammersleyBlock.hammersley[i + 1]);

        // theta from GGX BRDF
        float theta = atan(roughness * sqrt(hammersleyRand.y) / sqrt(1.0 - hammersleyRand.y));
        // skew the random numbers to match DistributionGGX
        vec2 uv = vec2(hammersleyRand.x, theta / PI);

        // inverse map from textureCoord to direction Vector
        float u = 2.0 * PI * (0.5 - uv.x);
        float v = PI * uv.y;
        vec3 L = vec3(cos(u) * sin(v), sin(u) * sin(v), cos(v));
        L = normalize(L);

        // Rotation applied to L
        // Note: (omega)w_K is a unit vector but normailzing it anyways to fix black dots (try without to see)
        vec3 w_K = L.x * A + L.y * B + L.z * R;
        w_K = normalize(w_K);

        // helper variables
        vec3 H = normalize(V + w_K);
        float wKdotH = max(dot(w_K, H), 0.0);
        float NdotwK = max(dot(N, w_K), 0.0);

        // For the “filter” step of the paper, read this texel from the mipmap at calculated level
        // TODO: try dividing log2(DistributionGGX(H, N, roughness) by 4.0
        float firstTerm  = 0.5 * log2(bufferWidth * bufferHeight / float(N_Hammersley));
        float secondTerm = 0.5 * log2(Herron_DistributionGGX(N, H, roughness) / 4.0);
        float level = max(firstTerm - secondTerm, 0.0);

        // irradiance
        vec3 Li = textureLod(skySphere, CalculateUVBasedOnDirection(w_K), level).rgb;

        // Final specular calculation
        //vec3 numerator = GeometrySmith(N, V, w_K, roughness) * FresnelSchlick(wKdotH, Ks) * cos(theta) * Li;
        vec3 numerator = GeometrySmith(N, V, w_K, roughness) * FresnelSchickRoughness(wKdotH, F0, roughness) * max(dot(N, w_K), 0.0) * Li;
        float denominator = max(4.0 * NdotV * NdotwK, 0.0001);
        specular += (numerator) / (denominator);
    }
    
    // take average
    specular /= N_Hammersley;

    // read irradiance map (from Normal N)
    vec3 irradiance = texture(irradianceMap, CalculateUVBasedOnDirection(N)).rgb;
    vec3 diffuse    = (Kd / PI) * irradiance;

    // read ao
    float AmbientOcclusion = 1.0;
    if (useAO)
    {
        AmbientOcclusion = texture(ambientOcclusionMap, TexCoords).r;
    }

    vec3 ambient = Kd * diffuse;

    vec3 color = (ambient + diffuse + specular) * AmbientOcclusion;

    return color;
}

// ----------------------------------------------------------------------------
vec3 PhongLighting(vec3 FragPos, vec3 Normal, vec3 Diffuse, float Specular)
{
    // then calculate lighting as usual
    vec3 ambient    = vec3(0.5f, 0.5f, 0.5f) * Diffuse;
    vec3 viewDir    = normalize(cameraPosition - FragPos);

    float shininess = 16.0;

    // diffuse
    vec3 lightDir = normalize(-light.Direction);
    vec3 diffuse  = max(dot(Normal, lightDir), 0.0) * Diffuse * light.Color;

    // specular
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular   = light.Color * spec * Specular;

    vec3 color = ambient + (diffuse + specular) * 100.0;

    return color;
}