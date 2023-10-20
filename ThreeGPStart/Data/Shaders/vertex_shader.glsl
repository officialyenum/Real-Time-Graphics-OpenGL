#version 330

// TODO

uniform mat4 combined_xform;
uniform mat4 model_xform;

layout(location = 0) in vec3 vertex_positions;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 tex_coords;


out vec4 vertexColor;
out vec2 texCoord;


void main(void)
{
	gl_Position = combined_xform * model_xform * vec4(vertex_positions, 1.0);
	vertexColor = vec4(normals, 1.0);
	texCoord = tex_coords;
}