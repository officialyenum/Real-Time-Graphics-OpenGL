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

	void InitGeometry(int size);
	// Function to set point light positions
	void SetPointLightPositions();

	void RenderLight(GLuint& m_program, const Helpers::Camera& camera);

	//Render in Scene

protected:
	LightStruct m_mesh;
	std::vector<LightStruct> m_meshVector;
	GLuint fxaa_fbo_{ 0 };

	GLuint positionsVBO{ 0 };
	GLuint coloursVBO{ 0 };
	GLuint texcoordsVBO{ 0 };
	GLuint elementsEBO{ 0 };


	GLuint diffuseMap{ 0 };
	GLuint specularMap{ 0 };
	GLuint emissionMap{ 0 };


	// Positions of the point lights
	std::vector<glm::vec3> pointLightPositions;


};

inline Light::Light()
{
}

inline Light::~Light()
{
}
inline void Light::SetPointLightPositions()
{
	// Set your point light positions here as needed
	pointLightPositions = {
		glm::vec3(0.7f,  0.2f,  10.0f),
		glm::vec3(2.3f, -3.3f, 10.0f),
		glm::vec3(-4.0f,  2.0f, 12.0f),
		glm::vec3(0.0f,  0.0f, 13.0f)
	};
}

inline void Light::InitGeometry(int size)
{
	

}

inline void Light::RenderLight(GLuint& m_program, const Helpers::Camera& camera)
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
	// set shader program // Light
	glUseProgram(m_program);


	GLint viewPosLoc = glGetUniformLocation(m_program, "viewPos");
	glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
	// Set material properties
	glUniform1f(glGetUniformLocation(m_program, "material.shininess"), 32.0f);
	// Directional light
	glUniform3f(glGetUniformLocation(m_program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(glGetUniformLocation(m_program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(m_program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
	glUniform3f(glGetUniformLocation(m_program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

	// Point light 1
	glUniform3f(glGetUniformLocation(m_program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[0].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[0].quadratic"), 0.032f);

	// Point light 2
	glUniform3f(glGetUniformLocation(m_program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[1].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[1].quadratic"), 0.032f);

	// Point light 3
	glUniform3f(glGetUniformLocation(m_program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[2].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[2].quadratic"), 0.032f);

	// Point light 4
	glUniform3f(glGetUniformLocation(m_program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(m_program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[3].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(m_program, "pointLights[3].quadratic"), 0.032f);

	// SpotLight
	glUniform3f(glGetUniformLocation(m_program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
	glUniform3f(glGetUniformLocation(m_program, "spotLight.direction"), camera.GetLookVector().x, camera.GetLookVector().y, camera.GetLookVector().z);
	glUniform3f(glGetUniformLocation(m_program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(m_program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(m_program, "spotLight.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(m_program, "spotLight.quadratic"), 0.032f);
	glUniform1f(glGetUniformLocation(m_program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
	glUniform1f(glGetUniformLocation(m_program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

	// Send the combined matrix to the shader in a uniform
	glUniformMatrix4fv(glGetUniformLocation(m_program, "combined_xform"), 1, GL_FALSE, glm::value_ptr(combined_xform));

	// Send the model matrix to the shader in a uniform
	glm::mat4 model_xform = glm::mat4(1);
	model_xform = glm::translate(model_xform, glm::vec3(2.0f, 5.0f, 5.0f));
	GLfloat angle = 10.0f;
	model_xform = glm::rotate(model_xform, angle, glm::vec3(1.0f, 0.3f, 0.5f));
	glUniformMatrix4fv(glGetUniformLocation(m_program, "model_xform"), 1, GL_FALSE, glm::value_ptr(model_xform));

	// Bind our VAO and render
	for (const LightStruct mesh : m_meshVector)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.numElements, GL_UNSIGNED_INT, (void*)0);
	}

	// We now draw as many light bulbs as we have point lights.
	glBindVertexArray(lightVAO);
	for (GLuint i = 0; i < 4; i++)
	{
		model_xform = glm::mat4();
		model_xform = glm::translate(model_xform, pointLightPositions[i]);
		model_xform = glm::scale(model_xform, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_xform));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}
