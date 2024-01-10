#pragma once
#pragma once

#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "Mesh.h"
#include "ImageLoader.h"
#include "Camera.h"

struct FrameBufferStruct
{
	GLuint VAO{ 0 };
	GLuint numElements{ 0 };
	GLuint meshTexture{ 0 };
};

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void InitGeometry(GLuint& m_program);
	GLuint GetMSAAFbo() { return FBO; };
	void CreateFrameBuffer(GLuint& m_program);
	void BindFrameBuffer(GLuint& m_program);
	void UnBindFrameBuffer(GLuint& m_program);
	void DeleteFrameBuffer();

	//Render in Scene

protected:
	FrameBufferStruct FrameBuffer_mesh;
	std::vector<FrameBufferStruct> FrameBuffer_meshVector;
	GLuint fxaa_fbo_{ 0 };

	GLuint FBO{ 0 };
	GLuint framebufferTexture{ 0 };

	GLuint RBO{ 0 };

	GLuint FBVAO{ 0 };
	GLuint FBVBO{ 0 };

	GLuint MSAAIntermediateFBO{ 0 };
	GLuint MSAAScreenTexture{ 0 };



};

inline FrameBuffer::FrameBuffer()
{
}

inline FrameBuffer::~FrameBuffer()
{
}

inline void FrameBuffer::InitGeometry(GLuint& m_program)
{

	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(m_program);
	glUniform1i(glGetUniformLocation(m_program, "screenTexture"), 0);

}

inline void FrameBuffer::CreateFrameBuffer(GLuint& m_program)
{
	float rectangleVertices[] =
	{
		// Coords    // texCoords
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f,

		 1.0f,  1.0f,  1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f,  0.0f, 1.0f
	};

	// Prepare framebuffer rectangle VBO and VAO
	glGenVertexArrays(1, &FBVAO);
	glGenBuffers(1, &FBVBO);
	glBindVertexArray(FBVAO);
	glBindBuffer(GL_ARRAY_BUFFER, FBVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Create Frame Buffer Object
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Framebuffer Texture
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

	// Create Render Buffer Object
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	// Error checking framebuffer
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;
}

inline void FrameBuffer::BindFrameBuffer(GLuint& m_program)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// Specify the color of the background
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Enable depth testing since it's disabled when drawing the framebuffer rectangle
	glEnable(GL_DEPTH_TEST);

}

inline void FrameBuffer::UnBindFrameBuffer(GLuint& m_program)
{
	// Bind the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw the framebuffer rectangle

	glUseProgram(m_program);
	glBindVertexArray(FBVAO);
	glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
	glBindTexture(GL_TEXTURE_2D, framebufferTexture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

inline void FrameBuffer::DeleteFrameBuffer()
{
	glDeleteFramebuffers(1, &FBO);
}
