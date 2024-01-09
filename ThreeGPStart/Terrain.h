#pragma once
#pragma once

#include "ExternalLibraryHeaders.h"
#include "Helper.h"
#include "Mesh.h"
#include "ImageLoader.h"
#include "Camera.h"

struct TerrainStruct
{
	GLuint VAO{ 0 };
	GLuint numElements{ 0 };
	GLuint meshTexture{ 0 };
};

class Terrain
{
public:
	Terrain();
	~Terrain();

	void InitGeometry(int size);
	// Function to set point light positions
	void SetPointLightPositions();

	void RenderTerrain(GLuint& m_program, const Helpers::Camera& camera, glm::vec3 position, glm::mat4 combined_xform, glm::vec3& lightPos);

	//Render in Scene

protected:
	TerrainStruct m_mesh;
	std::vector<TerrainStruct> m_meshVector;
	GLuint fxaa_fbo_{ 0 };

	GLuint positionsVBO{ 0 };
	GLuint coloursVBO{ 0 };
	GLuint texcoordsVBO{ 0 };
	GLuint elementsEBO{ 0 };


	// Positions of the point lights
	std::vector<glm::vec3> pointLightPositions;


};

inline Terrain::Terrain()
{
}

inline Terrain::~Terrain()
{
}
inline void Terrain::SetPointLightPositions()
{
	// Set your point light positions here as needed
	pointLightPositions = {
		glm::vec3(0.7f,  0.2f,  10.0f),
		glm::vec3(2.3f, -3.3f, 10.0f),
		glm::vec3(-4.0f,  2.0f, 12.0f),
		glm::vec3(0.0f,  0.0f, 13.0f)
	};
}

inline void Terrain::InitGeometry(int size)
{
	GLuint numCellsX{ 4 };
	GLuint numCellsZ{ 4 };

	GLuint numVertsX = numCellsX + 1;
	GLuint numVertsZ = numCellsZ + 1;

	GLuint numTrisX = numCellsX * 2;
	GLuint numTrisZ = numCellsZ * 2;

	float cellSizeX = size / (float)numCellsX;
	float cellSizeZ = size / (float)numCellsZ;

	glm::vec3 startPos{ -size / 2.0f, 0, size / 2.0f};

	std::vector<glm::vec3> vertices;

	glm::vec2 startUVPos{0, 0};
	std::vector<glm::vec2> uvCoords;

	glm::vec3 startNormalPos{0, 1, 0};
	std::vector<glm::vec3> normals;
	for (GLuint z = 0; z < numVertsZ; z++)
	{
		for (GLuint x = 0; x < numVertsX; x++)
		{
			{   // Position
				glm::vec3 pos{startPos};
				pos.x += cellSizeX * x;
				pos.z -= cellSizeZ * z;
				pos.y = 0;

				vertices.push_back(pos);
			}

			{   // UV
				glm::vec2 posUV{startPos};

				posUV.x = static_cast<float>(x);
				posUV.y = static_cast<float>(z);

				uvCoords.push_back(posUV);
			}

			{   // Normals
				glm::vec3 pos{startNormalPos};
				normals.push_back(pos);
			}
		}
	}
	std::vector<GLuint> elements;
	bool toggle{ true };
	for (GLuint z = 0; z < numCellsZ; z++)
	{
		for (GLuint x = 0; x < numCellsX; x++)
		{
			GLuint startVertex = z * numVertsX + x;

			if (toggle)
			{
				// Triangle 1
				elements.push_back(startVertex);
				elements.push_back(startVertex + 1);
				elements.push_back(startVertex + 1 + numVertsX);

				// Triangle 2
				elements.push_back(startVertex);
				elements.push_back(startVertex + 1 + numVertsX);
				elements.push_back(startVertex + numVertsX);
			}
			else
			{
				// Triangle 1
				elements.push_back(startVertex);
				elements.push_back(startVertex + 1);
				elements.push_back(startVertex + numVertsX);

				// Triangle 2
				elements.push_back(startVertex + 1);
				elements.push_back(startVertex + 1 + numVertsX);
				elements.push_back(startVertex + numVertsX);
			}
			toggle = !toggle;
		}
		toggle = !toggle;
	}

	/*
			Create Vertex Buffer Object (VBO) to hold vertex positions for terrain
	*/

	GLuint positionsVBO;
	glGenBuffers(1, &positionsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	GLuint texcoordsVBO;
	glGenBuffers(1, &texcoordsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * uvCoords.size(), uvCoords.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint normalsVBO;

	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint elementsEBO;
	glGenBuffers(1, &elementsEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), elements.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_mesh.numElements = static_cast<GLuint>(elements.size());
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

	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
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

	// Todo: Load Image Texture;
	Helpers::ImageLoader imageLoader;
	imageLoader.Load("Data\\Textures\\planks.png");

	glGenTextures(1, &m_mesh.meshTexture);
	glBindTexture(GL_TEXTURE_2D, m_mesh.meshTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, viewportSize[2], viewportSize[3], 0, GL_RGB, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, imageLoader.Width(), imageLoader.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageLoader.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);


	glBindTexture(GL_TEXTURE_2D, 0);

}

inline void Terrain::RenderTerrain(GLuint& m_program, const Helpers::Camera& camera, glm::vec3 position, glm::mat4 combined_xform, glm::vec3& lightPos)
{

	// Use our program. Doing this enables the shaders we attached previously.
	// set shader program // Terrain
	glUseProgram(m_program);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "combined_xform"), 1, GL_FALSE, glm::value_ptr(combined_xform));

	glm::mat4 model_xform = glm::mat4(1);
	model_xform = glm::translate(model_xform, glm::vec3(position));

	glUniformMatrix4fv(glGetUniformLocation(m_program, "model_xform"), 1, GL_FALSE, glm::value_ptr(model_xform));

	glUniform3f(glGetUniformLocation(m_program, "light_pos"), lightPos.x, lightPos.y, lightPos.z);



	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_mesh.meshTexture);
	glUniform1i(glGetUniformLocation(m_program, "sampler_tex"), 0);


	// Bind our VAO and render
	for (const TerrainStruct mesh : m_meshVector)
	{
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.numElements, GL_UNSIGNED_INT, (void*)0);
	}
}
