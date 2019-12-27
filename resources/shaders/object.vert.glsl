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
    vec3 fpos;
} output;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);

    output.uv = uv;
    output.color = color;
    output.normal = mat3(transpose(inverse(model))) * normal;
    output.fpos = vec3(model * vec4(position, 1.0));
}
