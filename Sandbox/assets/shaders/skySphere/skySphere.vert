#version 460 core

layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 FragPos;

void main()
{
    FragPos = (model * vec4(position.x, position.y, position.z, 1.0)).xyz;

    gl_Position = projection * view * model * vec4(position, 1.0);
}