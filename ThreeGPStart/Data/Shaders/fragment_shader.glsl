#version 330

out vec4 fragment_colour;

in vec4 vertexColor;
in vec2 texCoord;

uniform sampler2D ourTexture;

void main(void)
{
	fragment_colour = texture(ourTexture, texCoord);
	//fragment_colour = vec4(1.0,1.0,0.0,1.0);
}