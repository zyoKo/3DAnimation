#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D shadowMap;

struct Light
{
    vec3 Direction;
    vec3 Color;
};

uniform Light light;
uniform vec3 cameraPosition;

uniform mat4 shadowMatrix;
uniform float shadowBias;

float PI = 3.1415f;

vec3 CholeskyDecomposition(float m11, float m12, float m13, float m22, float m23, float m33, float z1, float z2, float z3)
{
    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    float a = sqrt(m11);    
    float b = m12 / a;
    float c = m13 / a;
    float d = sqrt(m22 - (b * b));
    float e = (m23 - (b * c)) / d;
    float f = sqrt(m33 - (c * c) - (e * e)); 

    // we don't want 0.0 or less for these values
    if (a <= 0.0f) { a = 0.0001f; }
    if (d <= 0.0f) { d = 0.0001f; }
    if (f <= 0.0f) { f = 0.0001f; }

    float c1Cap = z1 / a;
    float c2Cap = (z2 - (b * c1Cap)) / d;
    float c3Cap = (z3 - (c * c1Cap) - (e * c2Cap)) / f;

    float c3 = c3Cap / f;
    float c2 = (c2Cap - (e * c3)) / d;
    float c1 = (c1Cap - (b * c2) - (c * c3)) / a;

    result.x = c1;
    result.y = c2;
    result.z = c3;

    return result;
}

float MomentShadowMap_Hamburger(vec4 moments, float depth) 
{
    vec4 bPrime = ((1.0f - shadowBias) * moments) + (shadowBias * vec4(0.5f, 0.5f, 0.5f, 0.5f));

    float zF  = depth;
    float zF2 = pow(zF, 2);

    // m11(1.0f), m12, m13, m22, m23, m33, z1(1.0f), z2(zF), z3(zF2)
    vec3 choleskyResult = CholeskyDecomposition(1.0f, bPrime.x, bPrime.y, bPrime.y, bPrime.z, bPrime.w, 1.0f, zF, zF2);

    float z2 = 0.0f;
    float z3 = 0.0f;

    // (Solve quadratic equation) a = c.z, b = c.y, c = c.x
    // roots = b^2 - 4 * (a * c)
    float a = choleskyResult.z;
    float b = choleskyResult.y;
    float c = choleskyResult.x;
    float discriminant = (b * b) - (4.0 * a * c);
    z2 = (-b - sqrt(discriminant)) / (2.0 * a);

    // z2 should always be the smaller root
    if (a < 0.0)
    {
        z3 = z2;
        z2 = (-b + sqrt(discriminant)) / (2.0 * a);
    }
    else
    {
        z3 = (-b + sqrt(discriminant)) / (2.0 * a);
    }

    float G;
    if (zF <= z2) 
    {
        G = 0.0;
    }
    else if (zF <= z3) 
    {
        float a = zF * z3 - bPrime.x * (zF + z3) + bPrime.y;
        float b = (z3 - z2) * (zF - z2);

        G = a / b;
    }
    else 
    {
        float a = z2 * z3 - bPrime.x * (z2 + z3) + bPrime.y;
        float b = (zF - z2) * (zF - z3);

        G = 1 - (a / b);
    }

    return G;
}

float DistributionGGX(vec3 N, vec3 H, float alpha) {
    float HN = dot(H, N);
    float NH = dot(N, H);

    float char_factor = 1.0f;
    if (HN <= 0.0f) {
        char_factor = 0.0f;
    }

    float tan_theta_m = sqrt(1.0f - (HN * HN)) / HN;
    float tan_sq = tan_theta_m * tan_theta_m;

    float alpha_sq = alpha * alpha;

    float D_GGX = char_factor * alpha_sq / (PI * pow(NH, 4.0f) * pow(alpha_sq + tan_sq, 2.0f));

    return D_GGX;
}

float G1GGX(vec3 N, vec3 V, vec3 H, float alpha) {
    float VH = dot(V, H);
    float VN = dot(V, N);

    if (VN > 1.0f) { return 1.0f; }

    float VH_by_VN = VH / VN;
    float char_factor = 1.0f;
    if (VH_by_VN <= 0.0f) {
        char_factor = 0.0f;
    }

    float tan_theta_v = sqrt(1.0f - (VN * VN)) / VN;
    float tan_sq = tan_theta_v * tan_theta_v;
    if (tan_sq <= 0.0001f) { return 1.0f; }

    float alpha_sq = alpha * alpha;

    float G1 = char_factor * 2.0f / (1.0f + (sqrt(1.0f + (alpha_sq * tan_sq))));

    return G1;
}

float GeometryGGX(vec3 N, vec3 L, vec3 V, vec3 H, float alpha) {
    float G_Smith_GGX = G1GGX(N, L, H, alpha) * G1GGX(N, V, H, alpha);

    return G_Smith_GGX;
}

vec3 ReflectionFresnelSchlick(float LH, vec3 Ks) {
    vec3 F = Ks + ((1.0f - Ks) * pow(1.0f - abs(LH), 5.0f));

    if (F.x < 0.0f) { F.x = 0.0f; }
    if (F.y < 0.0f) { F.y = 0.0f; }
    if (F.z < 0.0f) { F.z = 0.0f; }

    return F;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

float VarianceShadowMap(vec2 Moments, float depth)
{
    // Moments
    float M1 = Moments.x;
    float M2 = Moments.y;

    float p = 0.0;
    if (depth <= M1)
    {
        p = 1.0;
    }

    // Variance Calculations
    float variance = M2 - pow(M1, 2);
    variance = max(variance, 3e-6);

    // Calculate final result
    float d = depth - M1;
    float pMax = variance / (variance + pow(d, 2));
    pMax = max(p, pMax);

    return 1.0 - pMax;
}

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
    vec3 lightDir = normalize(-light.Direction);
    //vec3 lightDir   = normalize(light.Position - FragPos);
    vec3 diffuse    = max(dot(Normal, lightDir), 0.0) * Diffuse * light.Color;

    // specular
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec      = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular   = light.Color * spec * Specular;

    //FragColor = vec4(ambient + diffuse + specular, 1.0);

    // Shadow Calculation
    float shadow = 0.0f;
    vec4 shadowPos   = shadowMatrix * vec4(FragPos, 1.0f);
    vec3 shadowCoord = shadowPos.xyz / shadowPos.w;
    shadowCoord = shadowCoord * 0.5f + 0.5f;
    if (shadowPos.w > 0 && (shadowCoord.x >= 0.0f && shadowCoord.x <= 1.0f) && (shadowCoord.y >= 0.0f && shadowCoord.y <= 1.0f))
    {
        vec4 Moments = texture(shadowMap, shadowCoord.xy);
        //shadow = 1.0f - MomentShadowMap_Hamburger(Moments, shadowCoord.z);
        shadow = 1.0f - VarianceShadowMap(Moments.xy, shadowCoord.z);
    }

    FragColor = vec4(ambient + (diffuse + specular) * shadow, 1.0);
}