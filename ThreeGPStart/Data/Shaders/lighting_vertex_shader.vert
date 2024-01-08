#version 330

uniform mat4 combined_xform;
uniform mat4 model_xform;

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec3 vertex_normals;
layout (location=2) in vec2 vertex_texcoords;

out vec3 varying_normals;
out vec3 varying_position;
out vec2 varying_texcoords;

void main(void)
{	
	varying_position = vec3(model_xform * vec4(vertex_position, 1.0));
	varying_normals = mat3(transpose(inverse(model_xform))) * vertex_normals;
	varying_texcoords = vertex_texcoords;

	gl_Position = combined_xform * model_xform * vec4(vertex_position, 1.0);
}

