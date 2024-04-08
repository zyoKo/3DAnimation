#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    TexCoords = tex;

    vec4 worldPos = model * vec4(pos, 1.0);
    FragPos = worldPos.xyz;

    gl_Position = projection * view * model * vec4(pos, 1.0);
}