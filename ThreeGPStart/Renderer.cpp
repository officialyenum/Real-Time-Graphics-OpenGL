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

	ImGui::Checkbox("MSAA", &m_msaa);	// A checkbox linked to a member variable

	// Alternatively, you can use ImGui::InputFloat3 for direct input
	ImGui::InputFloat3("Red Jeep", glm::value_ptr(red_jeep_position));
	ImGui::InputFloat3("Army Jeep", glm::value_ptr(army_jeep_position));
	ImGui::InputFloat3("Terrain", glm::value_ptr(terrain_position));
	ImGui::InputFloat3("Wild West", glm::value_ptr(wild_west_position));
	ImGui::InputFloat3("Apple", glm::value_ptr(apple_position));
	ImGui::InputFloat3("Camera Position", glm::value_ptr(camera_position));
	ImGui::InputFloat3("Camera Rotation", glm::value_ptr(camera_rotation));

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
		west_program = glCreateProgram();

		// Load and create vertex and fragment shaders
		GLuint l_vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/terrain_vertex_shader.vert") };
		GLuint west_fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/west_fragment_shader.frag") };
		if (l_vertex_shader == 0 || west_fragment_shader == 0)
			return false;

		// Attach the vertex shader to this program (copies it)
		glAttachShader(west_program, l_vertex_shader);

		// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
		// Not needed if you use (location=0) in the vertex shader itself
		//glBindAttribLocation(m_program, 0, "vertex_position");

		// Attach the fragment shader (copies it)
		glAttachShader(west_program, west_fragment_shader);

		// Done with the originals of these as we have made copies
		glDeleteShader(l_vertex_shader);
		glDeleteShader(west_fragment_shader);

		// Link the shaders, checking for errors
		if (!Helpers::LinkProgramShaders(west_program))
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

	{
		// Create a new program (returns a unqiue id)
		b_program = glCreateProgram();

		// Load and create vertex and fragment shaders
		GLuint b_vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/bot_vertex_shader.vert") };
		GLuint b_fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/bot_fragment_shader.frag") };
		if (b_vertex_shader == 0 || b_fragment_shader == 0)
			return false;

		// Attach the vertex shader to this program (copies it)
		glAttachShader(b_program, b_vertex_shader);

		// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
		// Not needed if you use (location=0) in the vertex shader itself
		//glBindAttribLocation(m_program, 0, "vertex_position");

		// Attach the fragment shader (copies it)
		glAttachShader(b_program, b_fragment_shader);

		// Done with the originals of these as we have made copies
		glDeleteShader(b_vertex_shader);
		glDeleteShader(b_fragment_shader);

		// Link the shaders, checking for errors
		if (!Helpers::LinkProgramShaders(b_program))
			return false;
	}
	
	{
		// Create a new program (returns a unqiue id)
		depth_program = glCreateProgram();

		// Load and create vertex and fragment shaders
		GLuint depth_buffer_vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/depth_buffer_vertex_shader.vert") };
		GLuint depth_buffer_fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/depth_buffer_fragment_shader.frag") };
		if (depth_buffer_vertex_shader == 0 || depth_buffer_fragment_shader == 0)
			return false;

		// Attach the vertex shader to this program (copies it)
		glAttachShader(depth_program, depth_buffer_vertex_shader);

		// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
		// Not needed if you use (location=0) in the vertex shader itself
		//glBindAttribLocation(m_program, 0, "vertex_position");

		// Attach the fragment shader (copies it)
		glAttachShader(depth_program, depth_buffer_fragment_shader);

		// Done with the originals of these as we have made copies
		glDeleteShader(depth_buffer_vertex_shader);
		glDeleteShader(depth_buffer_fragment_shader);

		// Link the shaders, checking for errors
		if (!Helpers::LinkProgramShaders(depth_program))
			return false;
	}
	return true;
}

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry()
{
	// Load and compile shaders into m_program
	if (!CreateProgram())
		return false;

	//Jeep
	armyJeepInstance.InitGeometry("Data\\Models\\Jeep\\jeep_army.jpg");
	redJeepInstance.InitGeometry("Data\\Models\\Jeep\\jeep_rood.jpg");
	//Apple
	//botInstance.InitGeometry();
	//Apple
	appleInstance.InitGeometry("Data\\Models\\Apple\\2.jpg");
	//Apple
	wildWestInstance.InitGeometry(west_program, "Data\\Models\\WildWest\\base.png");

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

	camera_position = { glm::vec3( camera.GetPosition()) };
	camera_rotation = { glm::vec3(camera.GetRotation()) };
	// Clear buffers from previous frame
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render Jeep;
	{
		armyJeepInstance.RenderJeep(m_program, camera, glm::vec3(army_jeep_position));
		redJeepInstance.RenderJeep(m_program, camera, glm::vec3(red_jeep_position));
	}
	//Render Bot;
	/*{
		botInstance.RenderBot(b_program, camera);
	}*/
	//Render Terrain;
	{
		terrainInstance.RenderTerrain(l_program, camera);
	}
	//Render Apple;
	{

		appleInstance.RenderApple(a_program, camera, glm::vec3(apple_position));
	}
	//Render Wild West;
	{
		wildWestInstance.RenderWildWest(a_program, camera, glm::vec3(wild_west_position));
	}
	//Render Depth Buffer;
	{

		depthBufferInstance.RenderDepthBuffer(depth_program, camera);
	}
	

}
