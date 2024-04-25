#version 460 core

out vec4 gAmbientOcclusion;

in vec2 TexCoords;

uniform vec3 cameraPosition;
uniform float s, k, R;

uniform sampler2D gPosition;
uniform sampler2D gNormal;

const float PI = 3.141592;

void main()
{
	int n = 10;
	float c = 0.1 * R;
	float delta = 0.001;

	ivec2 integerCoeff = ivec2(gl_FragCoord.xy);
    vec3 FragPos = texture(gPosition, TexCoords).xyz;
	vec3 Normal  = texture(gNormal, TexCoords).xyz;
	float d = length(cameraPosition - FragPos);

	float phi = (30 * integerCoeff.x) ^ integerCoeff.y + 10 * integerCoeff.x * integerCoeff.y;
	float sum = 0;
	for(int i = 0; i < n; ++i)
	{
		float alpha = (i + 0.5)/n;
		float h = alpha * R/d;
		float theta = 2.0 * PI * alpha * (7.0 * n / 9.0) + phi;

		vec3 Pi = texture(gPosition, TexCoords + h * vec2(cos(theta), sin(theta))).xyz;
		vec3 wi = Pi - FragPos;
		float di = length(cameraPosition - Pi);
		float H = 1.0;

		if (R - length(wi) < 0.0)
        {
			H = 0.0;
        }

		sum += max(0.0, dot(Normal, wi) - delta * di) * H / max(c * c, dot(wi, wi));
	}

    float numerator = 2.0 * PI * c;
    float denominator = n;

	sum *= numerator / denominator;

	vec3 result = vec3(pow((1.0 - s * sum), k));

	gAmbientOcclusion = vec4(result, 1.0);
}