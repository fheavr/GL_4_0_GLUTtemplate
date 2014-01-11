#version 330

// Copyright (c) Russell Gillette
// December 2013

in vec3 LightIntensity;

layout (location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(LightIntensity, 1.0);
}