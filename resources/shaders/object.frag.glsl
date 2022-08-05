#version 330 core

in Data {
    vec2 uv;
    vec4 color;
    vec3 normal;
    vec3 position;
    mat4 model;
} fragment;

out vec4 color0;

uniform sampler2D image;

uniform vec3  u_LightPos          = vec3(5000.0, -10000.0, 0.0);
uniform vec3  u_CameraPos         = vec3(0.0, 0.0, 0.0);
uniform float u_Attenuation       = 0.4;

uniform float u_DiffuseIntensity  = 0.8;

uniform vec3  u_AmbientColor      = vec3(1.0, 1.0, 1.0);
uniform float u_AmbientIntensity  = 1.5;

uniform vec3  u_SpecularColor     = vec3(1.0, 1.0, 1.0);
uniform float u_SpecularIntensity = 0.9;
uniform int   u_SpecularShininess = 32;

vec3 calcDiffuse()
{
    return fragment.color.xyz * u_DiffuseIntensity;
}

vec3 calcAmbient(vec3 normal, vec3 lightDir)
{
    float c = max(dot(normal, lightDir), 0.0);

    return c * u_AmbientColor * u_AmbientIntensity;
}

vec3 calcSpecular(vec3 normal, vec3 lightDir, vec3 viewDir)
{
    vec3 halfDir = normalize(lightDir + viewDir);
    float c = 1.0 + pow(max(dot(halfDir, normal), 0.0), u_SpecularShininess);

    return c * u_SpecularColor * u_SpecularIntensity;
}

void main()
{
    vec3 normal = normalize(transpose(inverse(mat3(fragment.model))) * fragment.normal);
    vec3 position = vec3(fragment.model * vec4(fragment.position, 1.0));

    vec3 lightDir = normalize(u_LightPos - position);
    vec3 viewDir = normalize(u_CameraPos - position);

    vec3 diffuse  = calcDiffuse();
    vec3 ambient  = calcAmbient(normal, lightDir);
    vec3 specular = calcSpecular(normal, lightDir, viewDir);

    color0 = vec4(texture(image, fragment.uv).xyz * u_Attenuation * (diffuse + ambient) * specular, fragment.color.w);
}
