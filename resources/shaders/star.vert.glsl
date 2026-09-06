#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec4 tint;

uniform mat4 mvp;

out vec2 uv;
out vec4 color;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    uv = texcoord;
    color = tint;
}
