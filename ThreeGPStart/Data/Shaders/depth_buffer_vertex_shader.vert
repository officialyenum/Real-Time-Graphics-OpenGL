#version 330 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

// Values that stay constant for the whole mesh.
uniform mat4 depthMVP;


out vec2 TexCoords;

void main(){
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos,1);
}