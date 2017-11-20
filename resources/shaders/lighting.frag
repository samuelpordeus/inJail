#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 color;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Light light;
uniform Material material;
uniform sampler2D tex;

void main()
{
    // Ambient ---
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse ---
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * material.diffuse * diff;

    // Specular ---
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.color * material.specular * spec;

    color = vec4(texture(tex, TexCoords)) * vec4((ambient + diffuse + specular) * 0.8, 1.0);
}
