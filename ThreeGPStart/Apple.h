#pragma once

#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "Mesh.h"
#include "ImageLoader.h"
#include "Camera.h"

struct AppleStruct
{
	GLuint VAO{ 0 };
	GLuint numElements{ 0 };
	GLuint meshTexture{ 0 };
};

class  Apple
{
public:
	Apple();
	~Apple();

	void InitGeometry(const std::string texture);
	void RenderApple(GLuint& m_program, const Helpers::Camera& camera, glm::vec3 position);

	//Render in Scene

protected:
	AppleStruct m_mesh;
	std::vector< AppleStruct> m_meshVector;
	GLuint fxaa_fbo_{ 0 };
	// Light attributes
	glm::vec3 lightPos;

	GLuint positionsVBO{ 0 };
	GLuint coloursVBO{ 0 };
	GLuint texcoordsVBO{ 0 };
	GLuint elementsEBO{ 0 };

};

inline  Apple::Apple()
{
	lightPos = glm::vec3(0.7f, 0.2f, 2.0f);
}

inline  Apple::~Apple()
{
}

inline void Apple::InitGeometry(const std::string texture)
{
	std::cout << "---------------------------------" << "\n";
	Helpers::ModelLoader loader;
	loader.LoadFromFile("Data\\Models\\Apple\\apple.obj");

	// Todo: Load Image Texture;
	Helpers::ImageLoader imageLoader;
	imageLoader.Load(texture);


	// Now we can loop through all the mesh in the loaded model:
	for (const Helpers::Mesh& mesh : loader.GetMeshVector())
	{
		std::cout << mesh.name << " + MatIndex: " << mesh.materialIndex << "\n";

		m_mesh.numElements = static_cast<GLuint>(mesh.elements.size());
		/*
			Create Vertex Buffer Object (VBO) to hold vertex positions
		*/

		glGenBuffers(1, &positionsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glGenBuffers(1, &coloursVBO);
		glBindBuffer(GL_ARRAY_BUFFER, coloursVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.normals.size(), mesh.normals.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &texcoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.uvCoords.size(), mesh.uvCoords.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &elementsEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLuint>(sizeof(GLuint) * mesh.elements.size()), mesh.elements.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		// VAO

		glGenVertexArrays(1, &m_mesh.VAO);
		glBindVertexArray(m_mesh.VAO);


		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,                  // attribute 0
			3,                  // size in bytes of each item in the stream
			GL_FLOAT,           // type of the item
			GL_FALSE,           // normalized or not (advanced)
			0,                  // stride (advanced)
			(void*)0            // array buffer offset (advanced)
		);


		glBindBuffer(GL_ARRAY_BUFFER, coloursVBO);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,                  // attribute 0
			3,                  // size in bytes of each item in the stream
			GL_FLOAT,           // type of the item
			GL_FALSE,           // normalized or not (advanced)
			0,                  // stride (advanced)
			(void*)0            // array buffer offset (advanced)
		);

		glBindBuffer(GL_ARRAY_BUFFER, texcoordsVBO);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(
			2,                  // attribute 0
			2,                  // size in bytes of each item in the stream
			GL_FLOAT,           // type of the item
			GL_FALSE,           // normalized or not (advanced)
			0,                  // stride (advanced)
			(void*)0            // array buffer offset (advanced)
		);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);


		// Clear VAO binding
		glBindVertexArray(0);

		m_meshVector.push_back(m_mesh);

	}
	std::cout << "---------------------------------" << "\n";



	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);

	//FXAA Framebuffer + Texture
	glGenFramebuffers(1, &fxaa_fbo_);
	glGenTextures(1, &m_mesh.meshTexture);
	glBindTexture(GL_TEXTURE_2D, m_mesh.meshTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, viewportSize[2], viewportSize[3], 0, GL_RGB, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, imageLoader.Width(), imageLoader.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageLoader.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);
}

inline void Apple::RenderApple(GLuint& m_program, const Helpers::Camera& camera, glm::vec3 position)
{


	// Compute viewport and projection matrix
	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);
	const float aspect_ratio = viewportSize[2] / (float)viewportSize[3];

	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspect_ratio, 1.0f, 4000.0f);

	// Compute camera view matrix and combine with projection matrix for passing to shader
	glm::mat4 view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());
	glm::mat4 combined_xform = projection_xform * view_xform;

	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(m_program);

	// Send the combined matrix to the shader in a uniform
	glUniformMatrix4fv(glGetUniformLocation(m_program, "combined_xform"), 1, GL_FALSE, glm::value_ptr(combined_xform));

	glm::mat4 model_xform = glm::mat4(1);
	model_xform = glm::translate(model_xform, glm::vec3(position));

	// Send the model matrix to the shader in a uniform
	glUniformMatrix4fv(glGetUniformLocation(m_program, "model_xform"), 1, GL_FALSE, glm::value_ptr(model_xform));


	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, m_mesh.meshTexture);
	glUniform1i(glGetUniformLocation(m_program, "sampler_tex"), 0);

	// Bind our VAO and render

	for (const  AppleStruct mesh : m_meshVector)
	{
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.numElements, GL_UNSIGNED_INT, (void*)0);
	}
}
