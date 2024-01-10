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
    // vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec4 color;
};

in vec3 varying_position;
in vec3 varying_normals;
in vec2 varying_texcoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(varying_normals);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(viewPos - varying_position);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(material.diffuse, varying_texcoords) * (diffuse + ambient) + texture(material.specular, varying_texcoords).r * specular) * light.color;
}

void main() {
    // Ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, varying_texcoords));
    
//     Diffuse
    vec3 norm = normalize(varying_normals);
    vec3 lightDir = normalize(light.position - varying_position);
    // vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, varying_texcoords));
    
//     Specular
    vec3 viewDir = normalize(viewPos - varying_position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, varying_texcoords));
    color = vec4(ambient + diffuse + specular, 1.0f);

    
	// outputs final color
//	float depth = logisticDepth(gl_FragCoord.z);
//	color = direcLight() * (1.0f - depth) + vec4(depth * vec3(0.85f, 0.85f, 0.90f), 1.0f);
//	color = direcLight();
}