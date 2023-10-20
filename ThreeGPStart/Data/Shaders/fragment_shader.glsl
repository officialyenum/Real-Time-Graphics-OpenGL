#version 330

uniform vec4 diffuse_colour;

in vec3 varying_normals;
in vec2 varying_texcoords;


out vec4 fragment_colour;

void main(void)
{
	vec3 N = (varying_normals + 0.5);
	fragment_colour = vec4(N, 1.0);
}