#include "Renderer.h"
#include "Camera.h"
#include "ImageLoader.h"



std::string MaterialIndexArray[2] =
{
	"a","b"
};

Renderer::Renderer()
{

}

// On exit must clean up any OpenGL resources e.g. the program, the buffers
Renderer::~Renderer()
{
	// TODO: clean up any memory used including OpenGL objects via glDelete* calls
	glDeleteProgram(m_program);
	glDeleteBuffers(1, &m_VAO);
}

// Use IMGUI for a simple on screen GUI
void Renderer::DefineGUI()
{
	// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::Begin("3GP");						// Create a window called "3GP" and append into it.

	ImGui::Text("Visibility.");					// Display some text (you can use a format strings too)	

	ImGui::Checkbox("Wireframe", &m_wireframe);	// A checkbox linked to a member variable

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}

// Load, compile and link the shaders and create a program object to host them
bool Renderer::CreateProgram()
{
	{
		// Create a new program (returns a unqiue id)
		m_program = glCreateProgram();

		// Load and create vertex and fragment shaders
		GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/vertex_shader.vert") };
		GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/fragment_shader.frag") };
		if (vertex_shader == 0 || fragment_shader == 0)
			return false;

		// Attach the vertex shader to this program (copies it)
		glAttachShader(m_program, vertex_shader);

		// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
		// Not needed if you use (location=0) in the vertex shader itself
		//glBindAttribLocation(m_program, 0, "vertex_position");

		// Attach the fragment shader (copies it)
		glAttachShader(m_program, fragment_shader);

		// Done with the originals of these as we have made copies
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		// Link the shaders, checking for errors
		if (!Helpers::LinkProgramShaders(m_program))
			return false;
	}
	

	{
		l_program = glCreateProgram();

		// Load and create vertex and fragment shaders
		GLuint l_vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/terrain_vertex_shader.vert") };
		GLuint l_fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/terrain_fragment_shader.frag") };
		if (l_vertex_shader == 0 || l_fragment_shader == 0)
			return false;

		// Attach the vertex shader to this program (copies it)
		glAttachShader(l_program, l_vertex_shader);

		// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
		// Not needed if you use (location=0) in the vertex shader itself
		//glBindAttribLocation(m_program, 0, "vertex_position");

		// Attach the fragment shader (copies it)
		glAttachShader(l_program, l_fragment_shader);

		// Done with the originals of these as we have made copies
		glDeleteShader(l_vertex_shader);
		glDeleteShader(l_fragment_shader);

		// Link the shaders, checking for errors
		if (!Helpers::LinkProgramShaders(l_program))
			return false;
	}

	{
		// Create a new program (returns a unqiue id)
		a_program = glCreateProgram();

		// Load and create vertex and fragment shaders
		GLuint a_vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/apple_vertex_shader.vert") };
		GLuint a_fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/apple_fragment_shader.frag") };
		if (a_vertex_shader == 0 || a_fragment_shader == 0)
			return false;

		// Attach the vertex shader to this program (copies it)
		glAttachShader(a_program, a_vertex_shader);

		// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
		// Not needed if you use (location=0) in the vertex shader itself
		//glBindAttribLocation(m_program, 0, "vertex_position");

		// Attach the fragment shader (copies it)
		glAttachShader(a_program, a_fragment_shader);

		// Done with the originals of these as we have made copies
		glDeleteShader(a_vertex_shader);
		glDeleteShader(a_fragment_shader);

		// Link the shaders, checking for errors
		if (!Helpers::LinkProgramShaders(a_program))
			return false;
	}
	

	return true;
}

void Renderer::CreateTerrain(int size)
{
	GLuint numCellsX{ 3 };
	GLuint numCellsZ{ 3 };

	GLuint numVertsX = numCellsX + 1;
	GLuint numVertsZ = numCellsZ + 1;

	GLuint numTrisX = numCellsX * 2;
	GLuint numTrisZ = numCellsZ;

	float cellSizeX = size / (float)numCellsX;
	float cellSizeZ = size / (float)numCellsZ;

	glm::vec3 startPos{ -size / 2.0f, 0, size / 2.0f};

	std::vector<glm::vec3> vertices;
	for (GLuint z = 0; z < numVertsZ; z++)
	{
		for (GLuint x = 0; x < numVertsX; x++)
		{
			glm::vec3 pos{startPos}; 
			pos.x += cellSizeX * x;
			//pos.y = (float)(rand()%100);
			pos.z += cellSizeX * z;

			vertices.push_back(pos);
		}
	}
	bool toggle{ true };
	std::vector<GLuint> elements;
	for (GLuint z = 0; z < numVertsZ; z++)
	{
		for (GLuint x = 0; x < numVertsX; x++)
		{
			GLuint startVertex = z * numVertsX + x;
			if (toggle) {
				// Triangle 1
				elements.push_back(startVertex);
				elements.push_back(startVertex + 1);
				elements.push_back(startVertex + 1 + numVertsX);

				// Triangle 2
				elements.push_back(startVertex);
				elements.push_back(startVertex + 1 + numVertsX);
				elements.push_back(startVertex + numVertsX);
			}
			else {
				// Triangle 1
				elements.push_back(startVertex);
				elements.push_back(startVertex + 1);
				elements.push_back(startVertex + 1 + numVertsX);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	GLuint texcoordsVBO;
	glGenBuffers(1, &texcoordsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texcoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint elementsEBO;
	glGenBuffers(1, &elementsEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * elements.size(), &elements[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// VAO

	glGenVertexArrays(1, &m_terrain_VAO);
	glBindVertexArray(m_terrain_VAO);


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

	glBindBuffer(GL_ARRAY_BUFFER, texcoordsVBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                  // attribute 0
		2,                  // size in bytes of each item in the stream
		GL_FLOAT,           // type of the item
		GL_FALSE,           // normalized or not (advanced)
		0,                  // stride (advanced)
		(void*)0            // array buffer offset (advanced)
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);


	// Clear VAO binding
	glBindVertexArray(0);
}

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry()
{
	// Load and compile shaders into m_program
	if (!CreateProgram())
		return false;

	//Jeep
	jeepInstance.InitGeometry();
	//Apple
	appleInstance.InitGeometry();

	//Terrain
	terrainInstance.SetPointLightPositions();
	terrainInstance.InitGeometry(10000);

	return true;
}

// Render the scene. Passed the delta time since last called.
void Renderer::Render(const Helpers::Camera& camera, float deltaTime)
{
	//glBindFramebuffer(GL_FRAMEBUFFER, fxaa_fbo_);

	// Configure pipeline settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Wireframe mode controlled by ImGui
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Clear buffers from previous frame
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render Jeep;
	{
		jeepInstance.RenderJeep(m_program, camera);
	}
	//Render Terrain;
	{
		terrainInstance.RenderTerrain(l_program, camera);
	}
	//Render Apple;
	{

		appleInstance.RenderApple(a_program, camera);
	}
	

}
