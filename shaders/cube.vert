#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 vertColor;

uniform mat4 mvp;

void main()
{
    // using row-column matrices
    gl_Position = vec4(aPos, 1.0) * mvp;
    vertColor   = aColor;
}