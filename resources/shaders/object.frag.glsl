#version 330 core

in Data {
    vec2 uv;
    vec4 color;
    vec3 normal;
    vec3 fpos;
} input;

out vec4 color;

uniform sampler2D image;

void main()
{
    vec3 lightColor = vec3(0.9, 1.0, 0.85);
    vec3 lightPosition = vec3(0.0, -5000.0, -5000.0);
    vec3 lightDirection = normalize(lightPosition - input.fpos);

    vec3 n = normalize(input.normal);
    vec3 diffuse = max(dot(n, lightDirection), 0.0) * lightColor;

    color = texture(image, input.uv) * (0.5 * input.color + vec4(diffuse, input.color.w));
}
