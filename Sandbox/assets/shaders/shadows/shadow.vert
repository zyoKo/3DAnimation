#version 460 core

layout (location = 0) in vec3 position;

uniform mat4 lightProjection;
uniform mat4 lightView;
uniform mat4 model;

out float depth;

void main()
{
    vec4 shadowPosition = lightProjection * lightView * model * vec4(position, 1.0);
    depth = (shadowPosition.xyz / shadowPosition.w).z;

    gl_Position = shadowPosition;
}