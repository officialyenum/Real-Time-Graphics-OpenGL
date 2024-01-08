#version 330 core

in vec2 varying_texcoords;
in vec3 varying_normals;
in vec3 varying_position;

out vec4 FragColor;

uniform vec4 lightColor;
uniform vec3 lightPos;

uniform sampler2D sampler_tex;

void main() {
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor.rgb;

    // Diffuse lighting
    vec3 norm = normalize(varying_normals);
    vec3 lightDir = normalize(lightPos - varying_position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor.rgb;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-varying_position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor.rgb;

    vec3 result = (ambient + diffuse + specular) * texture(sampler_tex, varying_texcoords).rgb;

    FragColor = vec4(result, 1.0);
}
