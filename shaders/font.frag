#version 330 core

layout(location = 0) out vec4 diffuseColor;

in vec2 texCoords;

uniform sampler2D text;
uniform vec4 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, texCoords).r);
    diffuseColor = textColor * sampled;
} 