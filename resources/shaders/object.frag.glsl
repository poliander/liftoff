#version 330 core

in Data {
    vec2 uv;
    vec4 color;
    vec3 normal;
    vec3 position;
    mat4 model;
} fragment;

out vec4 color;

uniform sampler2D image;

uniform vec3  u_LightPos          = vec3(1000.0f, -1000.0f, 0.0f);
uniform vec3  u_CameraPos         = vec3(0.0f, 0.0f, 0.0f);
uniform float u_Attenuation       = 0.333f;

uniform vec3  u_SpecularColor     = vec3(1.0f, 1.0f, 1.0f);
uniform float u_SpecularIntensity = 2.0f;
uniform int   u_SpecularShininess = 32;

uniform vec3  u_DiffuseColor      = vec3(1.0f, 1.0f, 1.0f);
uniform float u_DiffuseIntensity  = 2.0f;

vec3 calcDiffuse(vec3 normal, vec3 lightDir)
{
    float c = max(0.0, dot(normal, lightDir));

    return max(0.0, dot(normal, lightDir)) * u_DiffuseColor * u_DiffuseIntensity;
}

vec3 calcSpecular(vec3 normal, vec3 lightDir, vec3 viewDir)
{
    vec3 reflectionDir = reflect(-lightDir, normal);
    float c = pow(max(dot(viewDir, reflectionDir), 0.0), u_SpecularShininess);

    return c * u_SpecularColor * u_SpecularIntensity;
}

void main()
{
    vec3 normal = normalize(transpose(inverse(mat3(fragment.model))) * fragment.normal);
    vec3 position = vec3(fragment.model * vec4(fragment.position, 1.0));

    vec3 lightDir = normalize(u_LightPos - position);
    vec3 viewDir = normalize(u_CameraPos - position);

    vec4 ambient  = fragment.color;
    vec3 diffuse  = calcDiffuse(normal, lightDir);
    vec3 specular = calcSpecular(normal, lightDir, viewDir);

    color = vec4(texture(image, fragment.uv).xyz * u_Attenuation * (ambient.xyz + diffuse + specular), ambient.w);
}
