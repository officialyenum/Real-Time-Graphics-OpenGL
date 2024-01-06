#version 330

layout(location = 0) out vec4 fragment_colour;

uniform sampler2D terrain_tex;

in vec3 varying_colors;
in vec2 varying_texcoords;

void main(void)
{

	vec4 TexColor = texture(terrain_tex, varying_texcoords);
	fragment_colour = texture(terrain_tex, varying_texcoords) * vec4(varying_colors, 1.0);
}