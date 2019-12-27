#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 color;

out Data {
    vec2 uv;
    vec4 color;
    vec3 normal;
    vec3 position;
    mat4 model;
} fragment;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);

    fragment.uv = uv;
    fragment.color = color;
    fragment.normal = normal;
    fragment.position = position;
    fragment.model = model;
}
