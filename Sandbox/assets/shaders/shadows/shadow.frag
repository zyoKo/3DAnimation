#version 460 core

out vec4 FragColor;

in float depth;

void main()
{
    float relativeDepth       = 0.5 * (depth + 1.0);
    float relativeDepthSquare = relativeDepth * relativeDepth;      //pow(relativeDepth, 2);
    float relativeDepthCube   = relativeDepthSquare * relativeDepth;//pow(relativeDepth, 3);
    float relativeDepthForth  = relativeDepthCube * relativeDepth;  //pow(relativeDepth, 4);
    FragColor = vec4(relativeDepth, relativeDepthSquare, relativeDepthCube, relativeDepthForth);
}