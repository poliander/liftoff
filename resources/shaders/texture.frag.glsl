#version 330 core

in Data {
    vec2 uv;
} input;

out vec4 color0;

uniform sampler2D image;
uniform vec4 color;

void main()
{
    color0 = texture(image, input.uv) * color;
}

