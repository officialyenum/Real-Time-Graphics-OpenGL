#pragma once
#pragma once
#pragma once

#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "Mesh.h"
#include "ImageLoader.h"
#include "Camera.h"

struct ObjectStruct
{
	GLuint VAO{ 0 };
	GLuint numElements{ 0 };
	GLuint meshDiffuseTexture{ 0 };
	GLuint meshSpecularTexture{ 0 };
};

class  Object
{
public:
	Object();
	~Object();

	void InitGeometry(GLuint& m_program, const std::string& obj, const std::string& texture, const std::string& specularTexture);
	void RenderObject(GLuint& m_program, const Helpers::Camera& camera, glm::vec3& position, glm::mat4& combined_xform, glm::vec3& lightPos, std::vector<glm::vec3>& m_pointLightPositions);

	//Render in Scene

protected:
	ObjectStruct m_mesh;
	std::vector< ObjectStruct> m_meshVector;
	GLuint fxaa_fbo_{ 0 };

	GLuint positionsVBO{ 0 };
	GLuint coloursVBO{ 0 };
	GLuint texcoordsVBO{ 0 };
	GLuint elementsEBO{ 0 };

};

inline  Object::Object()
{

}

inline  Object::~Object()
{
}

inline void Object::InitGeometry(GLuint& m_program, const std::string& obj, const std::string& texture, const std::string& specularTexture)
{
	std::cout << "---------------------------------" << "\n";
	Helpers::ModelLoader loader;
	loader.LoadFromFile(obj);

	// Todo: Load Image Texture;
	Helpers::ImageLoader imageLoader;
	imageLoader.Load(texture);

	Helpers::ImageLoader specularLoader;
	specularLoader.Load(specularTexture);





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
	// base
	glGenTextures(1, &m_mesh.meshDiffuseTexture);
	glBindTexture(GL_TEXTURE_2D, m_mesh.meshDiffuseTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, viewportSize[2], viewportSize[3], 0, GL_RGB, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, imageLoader.Width(), imageLoader.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageLoader.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);



	//specular
	glGenTextures(1, &m_mesh.meshSpecularTexture);
	glBindTexture(GL_TEXTURE_2D, m_mesh.meshSpecularTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, viewportSize[2], viewportSize[3], 0, GL_RGB, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, specularLoader.Width(), specularLoader.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, specularLoader.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);




	// Set texture units
	glUseProgram(m_program);
	glUniform1i(glGetUniformLocation(m_program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(m_program, "material.specular"), 1);
}


inline void Object::RenderObject(GLuint& m_program, const Helpers::Camera& camera, glm::vec3& position, glm::mat4& combined_xform, glm::vec3& lightPos, std::vector<glm::vec3>& m_pointLightPositions)
{

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glUseProgram(m_program);

	glUniform3f(glGetUniformLocation(m_program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

	// Set lights properties

	// Set material properties
	glUniform1f(glGetUniformLocation(m_program, "material.shininess"), 32.0f);

	// Directional light

	glUniform3f(glGetUniformLocation(m_program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(m_program, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(m_program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(m_program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);



	// Point light 1
	glUniform3f(glGetUniformLocation(m_program, "pointLights[0].position"), m_pointLightPositions[0].x, m_pointLightPositions[0].y, m_pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[0].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[0].quadratic"), 0.032f);

	// Point light 2
	glUniform3f(glGetUniformLocation(m_program, "pointLights[1].position"), m_pointLightPositions[1].x, m_pointLightPositions[1].y, m_pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[1].ambient"), 0.95f, 0.85f, 0.85f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[1].linear"), 0.59f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[1].quadratic"), 0.032f);

	// Point light 3
	glUniform3f(glGetUniformLocation(m_program, "pointLights[2].position"), m_pointLightPositions[2].x, m_pointLightPositions[2].y, m_pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[2].diffuse"), 0.9f, 0.9f, 0.8f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[2].linear"), 0.89f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[2].quadratic"), 0.032f);

	// Point light 4
	glUniform3f(glGetUniformLocation(m_program, "pointLights[3].position"), m_pointLightPositions[3].x, m_pointLightPositions[3].y, m_pointLightPositions[3].z);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[3].ambient"), 1.f, 1.0f, 0.85f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[3].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[3].quadratic"), 0.032f);


	// SpotLight
	glUniform3f(glGetUniformLocation(m_program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
	glUniform3f(glGetUniformLocation(m_program, "spotLight.direction"), camera.GetLookVector().x, camera.GetLookVector().y, camera.GetLookVector().z);
	glUniform3f(glGetUniformLocation(m_program, "spotLight.ambient"), 1.0f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(m_program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "spotLight.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(m_program, "spotLight.quadratic"), 0.032f);
	glUniform1f(glGetUniformLocation(m_program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(m_program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

	// Send the combined matrix to the shader in a uniform
	glUniformMatrix4fv(glGetUniformLocation(m_program, "combined_xform"), 1, GL_FALSE, glm::value_ptr(combined_xform));

	glm::mat4 model_xform = glm::mat4(1);
	model_xform = glm::translate(model_xform, glm::vec3(position));

	// Send the model matrix to the shader in a uniform
	glUniformMatrix4fv(glGetUniformLocation(m_program, "model_xform"), 1, GL_FALSE, glm::value_ptr(model_xform));

	// Bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mesh.meshDiffuseTexture);

	// Bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_mesh.meshSpecularTexture);

	for (const ObjectStruct mesh : m_meshVector)
	{
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.numElements, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}

}
