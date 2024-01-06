#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"


struct Mesh
{
	GLuint VAO;
	GLuint numElements{ 0 };
};

struct Model
{
	std::vector<Mesh> mesh;
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
	// Program object - to host core shaders
	GLuint m_program{ 0 };
	// Program object - to host light shaders
	GLuint terrain_program{ 0 };

	// Vertex Array Object to wrap all render settings
	GLuint m_terrain_VAO{ 0 };
	// Vertex Array Object to wrap all render settings
	GLuint m_VAO{ 0 };

	// Number of elments to use when rendering
	GLuint m_numElements{ 0 };

	GLuint fxaa_fbo_{ 0 };
	GLuint fxaa_tex_{ 0 };
	GLuint terrain_tex_{ 0 };

	bool m_wireframe{ false };
	bool m_cullFace{ true };	

	std::vector<MeshStruct> m_meshVector;

public:
	Renderer();
	~Renderer();

	// Draw GUI
	void DefineGUI();

	// Create the program. This is the compiled shaders.
	bool CreateProgram();
	// Create the Terrain. This is the compiled shaders.
	void CreateTerrain(int size);

	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();

	GLuint DrawImage(GLuint textureID, const char* filename);

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

