#version 330 core

layout (location=0) in vec3 vertex_position;
layout (location=1) in vec3 vertex_normals;
layout (location=2) in vec2 vertex_texcoords;

out vec3 varying_normals;
out vec3 varying_position;
out vec2 varying_texcoords;
out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 combined_xform;
uniform mat4 model_xform;

void main()
{
    vs_out.FragPos = vec3(model * vec4(vertex_position, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model_xform))) * vertex_normals;
    vs_out.TexCoords = vertex_texcoords;
    vs_out.FragPosLightSpace = combined_xform * vec4(vs_out.FragPos, 1.0);
    gl_Position = projection * view * model_xform * vec4(vertex_position, 1.0);
}