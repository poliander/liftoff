#version 330 core

in vec2 uv;
in vec4 color;

out vec4 color0;

uniform sampler2D image;

void main()
{
    color0 = texture(image, uv) * color;
}
