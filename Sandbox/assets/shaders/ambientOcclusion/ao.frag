#version 460 core

out vec4 gAmbientOcclusion;

in vec2 TexCoords;

uniform vec3 cameraPosition;
uniform float scale, contrast, range;

uniform sampler2D gPosition;
uniform sampler2D gNormal;

const float PI = 3.141592;

void main()
{
	// read gBuffer Data
	vec3 FragPos = texture(gPosition, TexCoords).xyz;
	vec3 Normal  = texture(gNormal, TexCoords).xyz;

	int samples = 50;
	float c = 0.1 * range;
	float delta = 0.001;

	ivec2 iFragCoords = ivec2(gl_FragCoord.xy);
    
	float d = length(cameraPosition - FragPos);

	float phi = (30 * iFragCoords.x ^ iFragCoords.y) + 10 * iFragCoords.x * iFragCoords.y;
	float sum = 0;
	for(int i = 0; i < samples; ++i)
	{
		float alpha = (i + 0.5) / samples;
		float h = alpha * range / d;
		float theta = 2.0 * PI * alpha * (7.0 * samples / 9.0) + phi;

		vec2 newUV = TexCoords + h * vec2(cos(theta), sin(theta));

		vec3  Pi = texture(gPosition, newUV).xyz;
		vec3  wi = Pi - FragPos;
		float di = length(cameraPosition - Pi);
		float H  = 1.0;

		if (range - length(wi) < 0.0)
        {
			H = 0.0;
        }

		sum += max(0.0, dot(Normal, wi) - delta * di) * H / max(c * c, dot(wi, wi));
	}

    float numerator = 2.0 * PI * c;
    float denominator = samples;

	sum *= numerator / denominator;

	vec3 result = vec3(pow((1.0 - scale * sum), contrast));

	gAmbientOcclusion = vec4(result, 1.0);
}