#pragma once
#pragma once

#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "Mesh.h"
#include "ImageLoader.h"
#include "Camera.h"

struct MSAAFrameBufferStruct
{
	GLuint VAO{ 0 };
	GLuint numElements{ 0 };
	GLuint meshTexture{ 0 };
};

class MSAAFrameBuffer
{
public:
	MSAAFrameBuffer();
	~MSAAFrameBuffer();

	void InitGeometry(GLuint& m_program);
	GLuint GetMSAAFbo() { return MSAAFbo; };
	void RenderMSAAFrameBuffer(GLuint& m_program, const Helpers::Camera& camera, glm::mat4 combined_xform);
	void RenderMSAAFrameBufferAfter(GLuint& m_program);

	//Render in Scene

protected:
	MSAAFrameBufferStruct MSAAFrameBuffer_mesh;
	std::vector<MSAAFrameBufferStruct> MSAAFrameBuffer_meshVector;
	GLuint fxaa_fbo_{ 0 };

	GLuint MSAAFbo{ 0 };
	GLuint MSAATextureColorBufferMultiSampled{ 0 };

	GLuint MSAARbo{ 0 };

	GLuint MSAAQuadVAO{ 0 };
	GLuint MSAAQuadVBO{ 0 };

	GLuint MSAAIntermediateFBO{ 0 };
	GLuint MSAAScreenTexture{ 0 };

	

};

inline MSAAFrameBuffer::MSAAFrameBuffer()
{
}

inline MSAAFrameBuffer::~MSAAFrameBuffer()
{
}

inline void MSAAFrameBuffer::InitGeometry(GLuint& m_program)
{

	float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	// setup screen VAO
	glGenVertexArrays(1, &MSAAQuadVAO);
	glBindVertexArray(MSAAQuadVAO);

	glGenBuffers(1, &MSAAQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, MSAAQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);

	glUseProgram(m_program);
	glUniform1i(glGetUniformLocation(m_program, "screencapture"), 0);
	glUniform1i(glGetUniformLocation(m_program, "viewport_width"), 1280);
	glUniform1i(glGetUniformLocation(m_program, "viewport_height"), 720);
	// Always check that our framebuffer is ok
	/*if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;*/

		// configure MSAA framebuffer
		// --------------------------
	glGenFramebuffers(1, &MSAAFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, MSAAFbo);
	// create a multisampled color attachment texture
	glGenTextures(1, &MSAATextureColorBufferMultiSampled);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, MSAATextureColorBufferMultiSampled);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, 1280, 720, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, MSAATextureColorBufferMultiSampled, 0);


	// create a (also multisampled) renderbuffer object for depth and stencil attachments
	glGenRenderbuffers(1, &MSAARbo);
	glBindRenderbuffer(GL_RENDERBUFFER, MSAARbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, 1280, 720);
	glBindRenderbuffer(GL_RENDERBUFFER, 0); //unbind the render buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, MSAARbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

inline void MSAAFrameBuffer::RenderMSAAFrameBuffer(GLuint& m_program, const Helpers::Camera& camera, glm::mat4 combined_xform)
{
	glBindFramebuffer(GL_FRAMEBUFFER, MSAAFbo);
	// make sure we clear the framebuffer's content
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(m_program);

	// Send the combined matrix to the shader in a uniform
	glUniformMatrix4fv(glGetUniformLocation(m_program, "combined_xform"), 1, GL_FALSE, glm::value_ptr(combined_xform));

	// Send the model matrix to the shader in a uniform
	glUniformMatrix4fv(glGetUniformLocation(m_program, "model_xform"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

}

inline void MSAAFrameBuffer::RenderMSAAFrameBufferAfter(GLuint& m_program)
{
	// 3. now render quad with scene's visuals as its texture image
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	// draw Screen quad
	glUseProgram(m_program);
	glBindVertexArray(MSAAQuadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, MSAATextureColorBufferMultiSampled); // use the now resolved color attachment as the quad's texture
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
}
