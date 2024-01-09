#version 330 core


struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};
struct Light
{
    vec3 position;
    //vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 varying_position;
in vec3 varying_normals;
in vec2 varying_texcoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;


void main() {
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, varying_texcoords));
    
    // Diffuse
    vec3 norm = normalize(varying_normals);
    vec3 lightDir = normalize(light.position - varying_position);
    // vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, varying_texcoords));
    
    // Specular
    vec3 viewDir = normalize(viewPos - varying_position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, varying_texcoords));
    color = vec4(ambient + diffuse + specular, 1.0f);
}