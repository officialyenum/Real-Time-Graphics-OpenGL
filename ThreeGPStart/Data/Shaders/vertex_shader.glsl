#version 330

// TODO

uniform mat4 combined_xform;
uniform mat4 model_xform;


layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normals;
layout(location = 2) in vec2 aTexCoord;

out vec3 varying_normals;
out vec3 FragPos;
out vec2 TexCoord;


void main(void)
{
	varying_normals = vertex_normals;
	FragPos = vec3(model_xform * vec4(vertex_position, 1.0));
	TexCoord = aTexCoord;
	gl_Position = combined_xform * model_xform * vec4(vertex_position, 1.0);
}