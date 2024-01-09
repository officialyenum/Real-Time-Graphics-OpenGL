#version 330


out vec4 fragment_colour;

in vec2 varying_texcoords;

//uniform sampler2D screenTexture;
//notice the sampler
uniform sampler2DMS screencapture;
uniform int viewport_width;
uniform int viewport_height;

void main(void)
{
	//texelFetch requires a vec of ints for indexing (since we're indexing pixel locations)
	//texture coords is range [0, 1], we need range [0, viewport_dim].
	//texture coords are essentially a percentage, so we can multiply text coords by total size 
	ivec2 vpCoords = ivec2(viewport_width, viewport_height);
	vpCoords.x = int(vpCoords.x * varying_texcoords.x); 
	vpCoords.y = int(vpCoords.y * varying_texcoords.y);

	//do a simple average since this is just a demo
	vec4 sample1 = texelFetch(screencapture, vpCoords, 0);
	vec4 sample2 = texelFetch(screencapture, vpCoords, 1);
	vec4 sample3 = texelFetch(screencapture, vpCoords, 2);
	vec4 sample4 = texelFetch(screencapture, vpCoords, 3);
	fragment_colour = (sample1 + sample2 + sample3 + sample4) / 4.0f;


//    vec3 col = texture(screenTexture, varying_texcoords).rgb;
//    float grayscale = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
//    fragment_colour = vec4(vec3(grayscale), 1.0);

}
