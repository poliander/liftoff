#version 330 core

in vec2 uv;

out vec4 color0;

uniform sampler2D image;
uniform vec4 color;

void main()
{
    color0 = texture(image, uv) * color;
}
