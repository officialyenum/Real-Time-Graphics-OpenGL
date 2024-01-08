#version 330

uniform mat4 combined_xform;
uniform mat4 model_xform;

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec2 vertex_texcoords;

out vec2 varying_texcoords;

void main(void)
{	
	varying_texcoords = vertex_texcoords;

	gl_Position = vec4(vertex_position.x,vertex_position.y,0.0f, 1.0f);
}
