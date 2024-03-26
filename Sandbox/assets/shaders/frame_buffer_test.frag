#version 460

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;

void main()
{
	fragColor = texture(screenTexture, texCoords);
}