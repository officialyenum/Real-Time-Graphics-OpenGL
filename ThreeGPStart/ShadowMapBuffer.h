#pragma once
#pragma once
#pragma once

#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "Mesh.h"
#include "ImageLoader.h"
#include "Camera.h"

struct ShadowMapBufferStruct
{
	GLuint VAO{ 0 };
	GLuint numElements{ 0 };
	GLuint meshTexture{ 0 };
};

class ShadowMapBuffer
{
public:
	ShadowMapBuffer();
	~ShadowMapBuffer();

	void InitGeometry(GLuint& shadow_program, GLuint& depth_program);
	GLuint GetShadowFbo() { return ShadowFbo; };
	GLuint GetShadowTexture() { return ShadowTexture; };
	void RenderShadowMapBuffer(GLuint& m_program, glm::vec3& lightPos);
	void RenderShadowMapBufferAfter(GLuint& depth_program);
	void RenderQuad();

	//Render in Scene

protected:
	ShadowMapBufferStruct ShadowMapBuffer_mesh;
	std::vector<ShadowMapBufferStruct> ShadowMapBuffer_meshVector;
	GLuint fxaa_fbo_{ 0 };

	GLuint ShadowFbo{ 0 };
	GLuint ShadowTexture{ 0 };


	float near_plane = 1.0f;
	float far_plane = 7.5f;

	GLuint ShadowRbo{ 0 };

	GLuint ShadowQuadVAO{ 0 };
	GLuint ShadowQuadVBO{ 0 };

	GLuint ShadowIntermediateFBO{ 0 };
	GLuint ShadowScreenTexture{ 0 };



};

inline ShadowMapBuffer::ShadowMapBuffer()
{
}

inline ShadowMapBuffer::~ShadowMapBuffer()
{
}

inline void ShadowMapBuffer::InitGeometry(GLuint& shadow_program, GLuint& depth_program)
{

	

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &ShadowFbo);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &ShadowTexture);
	glBindTexture(GL_TEXTURE_2D, ShadowTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1028, 720, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glBindFramebuffer(GL_FRAMEBUFFER, ShadowFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowTexture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Intermediate SHADOW framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(shadow_program);

	// Send the combined matrix to the shader in a uniform
	glUniform1i(glGetUniformLocation(shadow_program, "diffuseTexture"), 0);
	glUniform1i(glGetUniformLocation(shadow_program, "shadowMap"), 1);

	glUseProgram(depth_program);
	glUniform1i(glGetUniformLocation(shadow_program, "depthMap"), 0);

}

inline void ShadowMapBuffer::RenderShadowMapBuffer(GLuint& m_program, glm::vec3& lightPos)
{

	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------

	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	glm::mat4 model_xform = glm::mat4(1.0);
	glm::mat4 depthMVP = lightSpaceMatrix * model_xform;


	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(m_program);

	// Send the combined matrix to the shader in a uniform
	glUniformMatrix4fv(glGetUniformLocation(m_program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

	// Send the model matrix to the shader in a uniform
	glUniformMatrix4fv(glGetUniformLocation(m_program, "model_xform"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	glUniformMatrix4fv(glGetUniformLocation(m_program, "depthMVP"), 1, GL_FALSE, &depthMVP[0][0]);


	glBindFramebuffer(GL_FRAMEBUFFER, ShadowFbo);
	glClear(GL_DEPTH_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);



	


}

inline void ShadowMapBuffer::RenderShadowMapBufferAfter(GLuint& depth_program)
{

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// render Depth map to quad for visual debugging
		// ---------------------------------------------

	glUseProgram(depth_program);

	glUniform1f(glGetUniformLocation(depth_program, "near_plane"), near_plane);
	glUniform1f(glGetUniformLocation(depth_program, "far_plane"), far_plane);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ShadowTexture);
	RenderQuad();
}


inline void ShadowMapBuffer::RenderQuad()
{
	
	if (ShadowQuadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &ShadowQuadVAO);
		glGenBuffers(1, &ShadowQuadVBO);
		glBindVertexArray(ShadowQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, ShadowQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(ShadowQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
