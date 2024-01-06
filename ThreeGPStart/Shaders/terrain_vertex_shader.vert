#version 330


uniform float gMinHeight;
uniform float gMaxHeight;
uniform mat4 model_xform;

layout (location=0) in vec3 vertex_position;
//layout (location=1) in vec3 vertex_normals;
layout (location=1) in vec2 vertex_texcoords;

out vec3 varying_colors;
out vec3 varying_position;
out vec2 varying_texcoords;

void main(void)
{	
	gl_Position = model_xform * vec4(vertex_position, 1.0);
	float DeltaHeight = gMaxHeight - gMinHeight;
	float HeightRatio = (vertex_position.y - gMinHeight) / DeltaHeight;
	float c = HeightRatio * 8.0 + 0.2;
	varying_colors = vec3(c);
	varying_texcoords = vertex_texcoords;

}