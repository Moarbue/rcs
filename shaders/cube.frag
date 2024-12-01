#version 330 core

layout(location = 0) out vec4 diffuseColor;

in vec4 vertColor;

void main()
{
    diffuseColor = vertColor;
}