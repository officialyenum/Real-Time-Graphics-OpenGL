#version 330

uniform vec4 diffuse_colour;
uniform sampler2D ourTexture;

in vec3 varying_normals;
in vec3 FragPos;
in vec2 TexCoord;


out vec4 fragment_colour;



void main(void)
{
	vec3 light_pos = vec3(0.0, 3000.0, 0.0);
	vec3 ambient_col = vec3(0.7, 0.2, 0.2);
	vec3 diff_col = vec3(1.0, 1.0, 1.0);
	vec3 red_col = vec3(1.0, 1.0, 0.0);

	vec3 tex_colour = texture(ourTexture, TexCoord).rgb;
	float ambientStrength = 0.4;
	vec3 ambient = ambientStrength * ambient_col;

	vec3 norm = normalize(varying_normals);
	vec3 lightDir = normalize(light_pos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * diff_col;

	vec3 result = (ambient + diffuse) * red_col;
	fragment_colour = texture(ourTexture, TexCoord) * vec4(result, 1.0);
	//fragment_colour = vec4(result, 1.0);
	//fragment_colour = vec4(tex_colour, 1.0);

	//(texture(ourTexture, TexCoord).rgb, 1.0);
}