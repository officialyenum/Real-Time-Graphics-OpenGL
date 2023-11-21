#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"

struct Model
{
	std::vector<Helpers::Mesh> mesh;
};

struct MeshStruct
{
	GLuint VAO;
	GLuint numElements;
	GLuint meshTexture;
};

class Renderer
{
private:
	std::vector<Model> m_models;
	// Program object - to host shaders
	GLuint m_program{ 0 };

	// Vertex Array Object to wrap all render settings
	GLuint m_VAO{ 0 };

	// Number of elments to use when rendering
	GLuint m_numElements{ 0 };

	GLuint m_tex{ 0 };

	bool m_wireframe{ false };
	bool m_cullFace{ true };

	std::vector<MeshStruct> m_meshVector;

public:
	Renderer();
	~Renderer();
	void DefineGUI();

	// Create the program. This is the compiled shaders.
	bool CreateProgram();

	void CreateTerrain(int size);

	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

