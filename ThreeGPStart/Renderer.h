#pragma once

#include "ExternalLibraryHeaders.h"

#include "Object/Jeep.h"
#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"
#include "Terrain.h"
#include "Apple.h"
#include "Bot.h"
#include "DepthFrameBuffer.h"
#include "WildWest.h"



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
	// Jeep Program object - to host core shaders
	GLuint m_program{ 0 };
	// Terrain Program object - to host light shaders
	GLuint l_program{ 0 };
	// Apple Program object - to host light shaders
	GLuint a_program{ 0 };
	// Apple Program object - to host light shaders
	GLuint b_program{ 0 };
	// Apple Program object - to host light shaders
	GLuint west_program{ 0 };
	// Apple Program object - to host light shaders
	GLuint depth_program{ 0 };
	

	// Vertex Array Object to wrap all render settings
	GLuint m_terrain_VAO{ 0 };
	// Vertex Array Object to wrap all render settings
	GLuint m_VAO{ 0 };

	// Number of elments to use when rendering
	GLuint m_numElements{ 0 };

	GLuint fxaa_fbo_{ 0 };
	GLuint fxaa_tex_{ 0 };

	//Jeep
	Jeep armyJeepInstance;
	glm::vec3 army_jeep_position { glm::vec3(-600.f, 0.f, 150.f) };
	Jeep redJeepInstance;
	glm::vec3 red_jeep_position { glm::vec3(-800.f, 0.f, -300.f) };

	
	DepthBuffer depthBufferInstance;
	Terrain terrainInstance;
	glm::vec3 terrain_position { glm::vec3(100.f, 100.f, 500.f) };
	Apple appleInstance;
	glm::vec3 apple_position { glm::vec3(0.f, 240.f, 300.f) };
	Bot botInstance;

	WildWest wildWestInstance;
	glm::vec3 wild_west_position { glm::vec3(-200.f, 100.f, 500.f) };

	//Camera
	glm::vec3 camera_position { glm::vec3(-0.1f, 285.f, -2930.f) };
	glm::vec3 camera_rotation { glm::vec3(-0.07f, 3.5f, 0.0f) };

	bool m_wireframe{ false };
	bool m_msaa{ false };
	bool m_cullFace{ true };	

	std::vector<MeshStruct> m_meshVector;

public:
	Renderer();
	~Renderer();

	// Draw GUI
	void DefineGUI();

	// Create the program. This is the compiled shaders.
	bool CreateProgram();

	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

