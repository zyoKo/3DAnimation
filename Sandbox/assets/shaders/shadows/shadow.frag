#version 460 core

out vec4 FragColor;

in float depth;

uniform float minDepth;
uniform float maxDepth;

void main()
{
    float relativeDepth = (depth - minDepth) / (maxDepth - minDepth);
    float relativeDepthSquare = pow(relativeDepth, 2);
    float relativeDepthCube   = pow(relativeDepth, 3);
    float relativeDepthForth  = pow(relativeDepth, 4);
    FragColor = vec4(relativeDepth, relativeDepthSquare, relativeDepthCube, relativeDepthForth);
}