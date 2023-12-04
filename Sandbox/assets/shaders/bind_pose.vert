#version 460 core

layout(location = 0) in vec3 jointPosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(jointPosition, 1.0);
}