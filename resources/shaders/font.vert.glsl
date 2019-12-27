#version 330 core

layout (location = 0) in vec4 vertex;

uniform mat4 mvp;

out Data {
    vec2 uv;
} output;

void main()
{
    gl_Position = mvp * vec4(vertex.xy, 0.0, 1.0);
    output.uv = vertex.zw;
}
