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

uniform vec3  u_LightPos          = vec3(5000.0f, -10000.0f, 0.0f);
uniform vec3  u_CameraPos         = vec3(0.0f, 0.0f, 0.0f);
uniform float u_Attenuation       = 0.4f;

uniform float u_AmbientIntensity  = 0.8f;

uniform vec3  u_DiffuseColor      = vec3(1.0f, 1.0f, 1.0f);
uniform float u_DiffuseIntensity  = 1.5f;

uniform vec3  u_SpecularColor     = vec3(1.0f, 1.0f, 1.0f);
uniform float u_SpecularIntensity = 1.0f;
uniform int   u_SpecularShininess = 32;

vec3 calcAmbient()
{
    return fragment.color.xyz * u_AmbientIntensity;
}

vec3 calcDiffuse(vec3 normal, vec3 lightDir)
{
    float c = max(dot(normal, lightDir), 0.0);

    return c * u_DiffuseColor * u_DiffuseIntensity;
}

vec3 calcSpecular(vec3 normal, vec3 lightDir, vec3 viewDir)
{
    vec3 reflectionDir = reflect(-lightDir, normal);
    float c = 1.0f + pow(max(dot(viewDir, reflectionDir), 0.0), u_SpecularShininess);

    return c * u_SpecularColor * u_SpecularIntensity;
}

void main()
{
    vec3 normal = normalize(transpose(inverse(mat3(fragment.model))) * fragment.normal);
    vec3 position = vec3(fragment.model * vec4(fragment.position, 1.0));

    vec3 lightDir = normalize(u_LightPos - position);
    vec3 viewDir = normalize(u_CameraPos - position);

    vec3 ambient  = calcAmbient();
    vec3 diffuse  = calcDiffuse(normal, lightDir);
    vec3 specular = calcSpecular(normal, lightDir, viewDir);

    color = vec4(texture(image, fragment.uv).xyz * u_Attenuation * (ambient + diffuse) * specular, fragment.color.w);
}
