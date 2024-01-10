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
#include "Light.h"
#include "MSAAFrameBuffer.h"
#include "ShadowMapBuffer.h"
#include "FrameBuffer.h"
#include "Object.h"



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
	// Object Program object - to host core shaders
	GLuint obj_program{ 0 };
	// Jeep Program object - to host core shaders
	GLuint m_program{ 0 };
	// Terrain Program object - to host light shaders
	GLuint terrain_program{ 0 };
	// Apple Program object - to host light shaders
	GLuint b_program{ 0 };
	// West Program object - to host light shaders
	GLuint west_program{ 0 };
	// Apple Program object - to host light shaders
	GLuint depth_program{ 0 };
	// Light Program object - to host light shaders
	GLuint light_source_program{ 0 };
	// Frame Buffer Program object - to host light shaders
	GLuint frame_buffer_program{ 0 };
	// MSAA Program object - to host light shaders
	GLuint msaa_program{ 0 };
	// MSAA Program object - to host light shaders
	GLuint shadow_program{ 0 };

	// Vertex Array Object to wrap all render settings
	GLuint m_terrain_VAO{ 0 };
	// Vertex Array Object to wrap all render settings
	GLuint m_VAO{ 0 };

	// Number of elments to use when rendering
	GLuint m_numElements{ 0 };

	GLuint fxaa_fbo_{ 0 };
	GLuint fxaa_tex_{ 0 };

	//Object Program
	Object armyJeepInstance;
	glm::vec3 army_jeep_position { glm::vec3(-600.f, 0.f, -200.f) };
	Object redJeepInstance;
	glm::vec3 red_jeep_position { glm::vec3(-800.f, 0.f, -800.f) };
	Object appleInstance;
	glm::vec3 apple_position { glm::vec3(0.f, 240.f, 300.f) };
	Object wildWestInstance;
	glm::vec3 wild_west_position { glm::vec3(-200.f, 100.f, 500.f) };
	Object caveInstance;
	glm::vec3 cave_position { glm::vec3(-400.f, 240.f, 300.f) };
	Object cargoInstance;
	glm::vec3 cargo_position { glm::vec3(400.f, 240.f, 300.f) };

	//West Program
	WildWest w_armyJeepInstance;
	WildWest w_redJeepInstance;
	WildWest w_appleInstance;
	WildWest w_wildWestInstance;
	WildWest w_caveInstance;
	WildWest w_cargoInstance;

	Terrain terrainInstance;
	glm::vec3 terrain_position { glm::vec3(100.f, 0.f, 500.f) };

	//Lights
	Light lightSource;
	glm::vec3 light_source_position { glm::vec3(lightSource.GetPosition()) };
	std::vector<glm::vec3> point_light_source_position { std::vector<glm::vec3>(lightSource.GetPointLightPositions()) };

	//Buffers
	FrameBuffer frameBuffer;
	MSAAFrameBuffer msaaFrameBuffer;
	DepthBuffer depthBufferInstance;
	ShadowMapBuffer shadowMapBufferInstance;

	//Camera
	glm::vec3 camera_position { glm::vec3(37.811f, 310.902f, 255.142) };
	glm::vec3 camera_rotation { glm::vec3(-0.010f, 3.727f, 0.0f) };
	/*glm::vec3 camera_position { glm::vec3(532.0f, 880.f, -2156.f) };
	glm::vec3 camera_rotation { glm::vec3(0.23f, 3.8f, 0.0f) };*/

	bool m_wireframe{ false };
	bool m_msaa{ true };
	bool m_cullFace{ false };
	bool m_depth{ true };
	bool m_counter_clockwise{ false };
	bool m_shadow_map{ false };
	bool m_single_light{ false };

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

