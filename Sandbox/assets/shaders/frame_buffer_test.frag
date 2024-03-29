#version 460

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D positionTexture;

void main()
{
	// Inversion
	//fragColor = vec4(vec3(1.0 - texture(positionTexture, texCoords)), 1.0);

	fragColor = texture(positionTexture, texCoords);
}