#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 tex;

out vec2 texCoords;

uniform mat4 proj;

void main()
{
    gl_Position = proj * vec4(pos, -1.0, 1.0);
    texCoords = tex;
} 