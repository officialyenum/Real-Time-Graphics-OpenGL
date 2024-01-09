#pragma once

#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "Mesh.h"
#include "ImageLoader.h"
#include "Camera.h"

struct DepthBufferStruct
{
	GLuint VAO{ 0 };
	GLuint numElements{ 0 };
	GLuint meshTexture{ 0 };
};

class DepthBuffer
{
public:
	DepthBuffer();
	~DepthBuffer();

	void InitGeometry();
	void RenderDepthBuffer(GLuint& m_program, const Helpers::Camera& camera);

	//Render in Scene

protected:
	DepthBufferStruct DepthBuffer_mesh;
	std::vector<DepthBufferStruct> DepthBuffer_meshVector;
	GLuint fxaa_fbo_{ 0 };

	GLuint depthMapFBO{ 0 };
	GLuint depthMap{ 0 };

};

inline DepthBuffer::DepthBuffer()
{
}

inline DepthBuffer::~DepthBuffer()
{
}

inline void DepthBuffer::InitGeometry()
{
	std::cout << "---------------------------------" << "\n";
	Helpers::ModelLoader loader;
	loader.LoadFromFile("Data\\Models\\DepthBuffer\\DepthBuffer.obj");

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	// ----------------
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	/*if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;*/
}

inline void DepthBuffer::RenderDepthBuffer(GLuint& m_program, const Helpers::Camera& camera)
{


	// Compute viewport and projection matrix
	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);
	const float aspect_ratio = viewportSize[2] / (float)viewportSize[3];

	glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);

	// Compute the MVP matrix from the light's point of view
	glm::mat4 projection_xform = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 view_xform = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 model_xform = glm::mat4(1.0);
	glm::mat4 combined_xform = projection_xform * view_xform;
	glm::mat4 depthMVP = combined_xform * model_xform;
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	glm::mat4 depthBiasMVP = biasMatrix * depthMVP;

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform

	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(m_program);

	glUniformMatrix4fv(glGetUniformLocation(m_program, "depthMVP"), 1, GL_FALSE, &depthMVP[0][0]);
	
}
