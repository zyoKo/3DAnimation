#version 460 core

out vec4 FragColor;

in vec3 FragPos;

uniform vec3 cameraPosition;

uniform sampler2D skySphere;

const float PI = 3.14159265359f;

vec2 CalculateUVBasedOnDirection(vec3 direction)
{
    float u = 0.5 - atan(direction.z, -direction.x) / (2.0 * PI);
    float v = acos(-direction.y) / PI;

    return vec2(u, v);
}

void main()
{
    // view direction
    vec3 V = normalize(cameraPosition - FragPos);

    vec2 uv = CalculateUVBasedOnDirection(V);

    vec3 diffuse = texture(skySphere, uv).rgb;

    // HDR tonemapping and Linear color space
    diffuse = diffuse / (diffuse + vec3(1.0));
    diffuse = pow(diffuse, vec3(1.0 / 2.2));

    FragColor = vec4(diffuse, 1.0);
}