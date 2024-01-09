#pragma once
#pragma once
#pragma once

#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "Mesh.h"
#include "ImageLoader.h"
#include "Camera.h"

struct LightStruct
{
	GLuint VAO{ 0 };
	GLuint numElements{ 0 };
	GLuint meshTexture{ 0 };
};

class Light
{
public:
	Light();
	~Light();

	void InitGeometry();

	void RenderLight(GLuint& m_program, const Helpers::Camera& camera, glm::mat4 combined_xform);


	glm::vec3 GetPosition() { return lightPos; };

	//Render in Scene

protected:
	LightStruct m_mesh;
	std::vector<LightStruct> m_meshVector;
	GLuint fxaa_fbo_{ 0 };

	glm::vec3 lightPos{ glm::vec3(0.0f, 500.0f, 0.0f) };

	GLuint positionsVBO{ 0 };
	GLuint lightVAO{ 0 };


	GLuint diffuseMap{ 0 };
	GLuint specularMap{ 0 };
	GLuint emissionMap{ 0 };

};

inline Light::Light()
{

}

inline Light::~Light()
{
}


inline void Light::InitGeometry()
{
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};
	
	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &positionsVBO);

	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(lightVAO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);

}

inline void Light::RenderLight(GLuint& m_program, const Helpers::Camera& camera, glm::mat4 combined_xform)
{
	// Use our program. Doing this enables the shaders we attached previously.
	// set shader program // Light
	glUseProgram(m_program);

	glUniform3f(glGetUniformLocation(m_program, "objectColor"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(m_program, "lightColor"), 1.0f, 1.0f, 1.0f);

	// Get location objects for the matrices on the lamp shader (these could be different on a different shader)

	// Send the combined matrix to the shader in a uniform
	glUniformMatrix4fv(glGetUniformLocation(m_program, "combined_xform"), 1, GL_FALSE, glm::value_ptr(combined_xform));

	// Send the model matrix to the shader in a uniform
	glm::mat4 model_xform = glm::mat4(1);
	model_xform = glm::translate(model_xform, glm::vec3(lightPos.x, lightPos.y, lightPos.z));
	model_xform = glm::scale(model_xform, glm::vec3(0.2f));
	glUniformMatrix4fv(glGetUniformLocation(m_program, "model_xform"), 1, GL_FALSE, glm::value_ptr(model_xform));



	// Draw the light object (using light's vertex attributes)
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}
