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

	ImGui::Checkbox("MSAA X8", &m_msaa);	// A checkbox linked to a member variable
	ImGui::Checkbox("Shadow Map", &m_shadow_map);	// A checkbox linked to a member variable
	ImGui::Checkbox("Cull Face", &m_cullFace);	// A checkbox linked to a member variable

	ImGui::Checkbox("Depth Test", &m_depth);	// A checkbox linked to a member variable
	ImGui::Checkbox("Single/Multi Light", &m_single_light);	// A checkbox linked to a member variable
	ImGui::Checkbox("Counter Clockwise", &m_counter_clockwise);	// A checkbox linked to a member variable



	// Alternatively, you can use ImGui::InputFloat3 for direct input
	ImGui::InputFloat3("Red Jeep", glm::value_ptr(red_jeep_position));
	ImGui::InputFloat3("Army Jeep", glm::value_ptr(army_jeep_position));
	ImGui::InputFloat3("Terrain", glm::value_ptr(terrain_position));
	ImGui::InputFloat3("Wild West", glm::value_ptr(wild_west_position));
	ImGui::InputFloat3("Cave", glm::value_ptr(cave_position));
	ImGui::InputFloat3("Cargo", glm::value_ptr(cargo_position));
	ImGui::InputFloat3("Apple", glm::value_ptr(apple_position));
	ImGui::InputFloat3("Camera Position", glm::value_ptr(camera_position));
	ImGui::InputFloat3("Camera Rotation", glm::value_ptr(camera_rotation));
	ImGui::InputFloat3("Scene Light Source Position", glm::value_ptr(light_source_position));
	ImGui::InputFloat3("Point Light 1 Position", glm::value_ptr(point_light_source_position[0]));
	ImGui::InputFloat3("Point Light 2 Position", glm::value_ptr(point_light_source_position[1]));
	ImGui::InputFloat3("Point Light 3 Position", glm::value_ptr(point_light_source_position[2]));
	ImGui::InputFloat3("Point Light 4 Position", glm::value_ptr(point_light_source_position[3]));

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}

// Load, compile and link the shaders and create a program object to host them
bool Renderer::CreateProgram()
{
	{
		// Create a light source program (returns a unqiue id)
		light_source_program = glCreateProgram();

		// Load and create vertex and fragment shaders
		GLuint light_vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/lamp_vertex_shader.vert") };
		GLuint light_fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/direction_light_fragment_shader.frag") };
		if (light_vertex_shader == 0 || light_fragment_shader == 0)
			return false;

		// Attach the vertex shader to this program (copies it)
		glAttachShader(light_source_program, light_vertex_shader);

		// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
		// Not needed if you use (location=0) in the vertex shader itself
		//glBindAttribLocation(m_program, 0, "vertex_position");

		// Attach the fragment shader (copies it)
		glAttachShader(light_source_program, light_fragment_shader);

		// Done with the originals of these as we have made copies
		glDeleteShader(light_vertex_shader);
		glDeleteShader(light_fragment_shader);

		// Link the shaders, checking for errors
		if (!Helpers::LinkProgramShaders(light_source_program))
			return false;
	}

	{
		// Create a jeep program (returns a unqiue id)
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
		//create a terrain program
		terrain_program = glCreateProgram();

		// Load and create vertex and fragment shaders
		GLuint l_vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/terrain_vertex_shader.vert") };
		GLuint l_fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/terrain_fragment_shader.frag") };
		if (l_vertex_shader == 0 || l_fragment_shader == 0)
			return false;

		// Attach the vertex shader to this program (copies it)
		glAttachShader(terrain_program, l_vertex_shader);

		// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
		// Not needed if you use (location=0) in the vertex shader itself
		//glBindAttribLocation(m_program, 0, "vertex_position");

		// Attach the fragment shader (copies it)
		glAttachShader(terrain_program, l_fragment_shader);

		// Done with the originals of these as we have made copies
		glDeleteShader(l_vertex_shader);
		glDeleteShader(l_fragment_shader);

		// Link the shaders, checking for errors
		if (!Helpers::LinkProgramShaders(terrain_program))
			return false;
	}

	{
		//create a Frame Buffer program
		obj_program = glCreateProgram();

		// Load and create vertex and fragment shaders
		GLuint obj_vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/object_vertex_shader.vert") };
		GLuint obj_fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/object_fragment_shader.frag") };
		if (obj_vertex_shader == 0 || obj_fragment_shader == 0)
			return false;

		// Attach the vertex shader to this program (copies it)
		glAttachShader(obj_program, obj_vertex_shader);

		// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
		// Not needed if you use (location=0) in the vertex shader itself
		//glBindAttribLocation(m_program, 0, "vertex_position");

		// Attach the fragment shader (copies it)
		glAttachShader(obj_program, obj_fragment_shader);

		// Done with the originals of these as we have made copies
		glDeleteShader(obj_vertex_shader);
		glDeleteShader(obj_fragment_shader);

		// Link the shaders, checking for errors
		if (!Helpers::LinkProgramShaders(obj_program))
			return false;
	}

	{
		//create a Frame Buffer program
		frame_buffer_program = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint frame_buffer_vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/frame_buffer_vertex_shader.vert") };
	GLuint frame_buffer_fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/frame_buffer_fragment_shader.frag") };
	if (frame_buffer_vertex_shader == 0 || frame_buffer_fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(frame_buffer_program, frame_buffer_vertex_shader);

	// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(frame_buffer_program, frame_buffer_fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(frame_buffer_vertex_shader);
	glDeleteShader(frame_buffer_fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(frame_buffer_program))
		return false;
	}

	{
		//create a MSAA program
		msaa_program = glCreateProgram();

		// Load and create vertex and fragment shaders
		GLuint msaa_vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/msaa_vertex_shader.vert") };
		GLuint msaa_fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/msaa_fragment_shader.frag") };
		if (msaa_vertex_shader == 0 || msaa_fragment_shader == 0)
			return false;

		// Attach the vertex shader to this program (copies it)
		glAttachShader(msaa_program, msaa_vertex_shader);

		// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
		// Not needed if you use (location=0) in the vertex shader itself
		//glBindAttribLocation(m_program, 0, "vertex_position");

		// Attach the fragment shader (copies it)
		glAttachShader(msaa_program, msaa_fragment_shader);

		// Done with the originals of these as we have made copies
		glDeleteShader(msaa_vertex_shader);
		glDeleteShader(msaa_fragment_shader);

		// Link the shaders, checking for errors
		if (!Helpers::LinkProgramShaders(msaa_program))
			return false;
	}

	{
		//create a Shadow program
		shadow_program = glCreateProgram();

		// Load and create vertex and fragment shaders
		GLuint shadow_buffer_vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/shadow_buffer_vertex_shader.vert") };
		GLuint shadow_buffer_fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/shadow_buffer_fragment_shader.frag") };
		if (shadow_buffer_vertex_shader == 0 || shadow_buffer_fragment_shader == 0)
			return false;

		// Attach the vertex shader to this program (copies it)
		glAttachShader(shadow_program, shadow_buffer_vertex_shader);

		// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
		// Not needed if you use (location=0) in the vertex shader itself
		//glBindAttribLocation(m_program, 0, "vertex_position");

		// Attach the fragment shader (copies it)
		glAttachShader(shadow_program, shadow_buffer_fragment_shader);

		// Done with the originals of these as we have made copies
		glDeleteShader(shadow_buffer_vertex_shader);
		glDeleteShader(shadow_buffer_fragment_shader);

		// Link the shaders, checking for errors
		if (!Helpers::LinkProgramShaders(shadow_program))
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
	//msaa
	//msaaFrameBuffer.InitGeometry(msaa_program);
	//shadow map
	shadowMapBufferInstance.InitGeometry(shadow_program, depth_program);
	//Lights
	lightSource.InitGeometry();
	lightSource.InitPointGeometry();
	//Point Light Source
	/*for (size_t i = 0; i < m_pointLightObjects.size(); ++i) {
		m_pointLightObjects[i].InitGeometry();
	}*/


	//Object
	armyJeepInstance.InitGeometry(west_program, "Data\\Models\\Jeep\\jeep.obj", "Data\\Models\\Jeep\\jeep_army.jpg", "");
	redJeepInstance.InitGeometry(west_program, "Data\\Models\\Jeep\\jeep.obj", "Data\\Models\\Jeep\\jeep_rood.jpg", "nullptr");
	appleInstance.InitGeometry(west_program, "Data\\Models\\Apple\\apple.obj", "Data\\Models\\Apple\\2.jpg", "");
	caveInstance.InitGeometry(west_program, "Data\\Models\\Cave\\cave.obj", "Data\\Models\\Cave\\diffuse.png", "Data\\Models\\Cave\\specular.png");
	cargoInstance.InitGeometry(west_program, "Data\\Models\\Bench\\bench.obj", "Data\\Models\\Bench\\diffuse.png", "Data\\Models\\Bench\\specular.png");
	wildWestInstance.InitGeometry(west_program, "Data\\Models\\WildWest\\WildWest.obj", "Data\\Models\\WildWest\\base.png", "Data\\Models\\WildWest\\specular.png");
	
	// West Program
	w_armyJeepInstance.InitGeometry(west_program, "Data\\Models\\Jeep\\jeep.obj", "Data\\Models\\Jeep\\jeep_army.jpg", "");
	w_redJeepInstance.InitGeometry(west_program, "Data\\Models\\Jeep\\jeep.obj", "Data\\Models\\Jeep\\jeep_rood.jpg", "nullptr");
	w_appleInstance.InitGeometry(west_program, "Data\\Models\\Apple\\apple.obj", "Data\\Models\\Apple\\2.jpg", "");
	w_caveInstance.InitGeometry(west_program, "Data\\Models\\Cave\\cave.obj", "Data\\Models\\Cave\\diffuse.png", "Data\\Models\\Cave\\specular.png");
	w_cargoInstance.InitGeometry(west_program, "Data\\Models\\Bench\\bench.obj", "Data\\Models\\Bench\\diffuse.png", "Data\\Models\\Bench\\specular.png");
	w_wildWestInstance.InitGeometry(west_program, "Data\\Models\\WildWest\\WildWest.obj", "Data\\Models\\WildWest\\base.png", "Data\\Models\\WildWest\\specular.png");

	//Terrain
	terrainInstance.InitGeometry(10000);

	return true;
}

// Render the scene. Passed the delta time since last called.
void Renderer::Render(const Helpers::Camera& camera, float deltaTime)
{
	//glBindFramebuffer(GL_FRAMEBUFFER, fxaa_fbo_);


	
	// Uses counter clock-wise standard

	// Wireframe mode controlled by ImGui
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	
	if (m_cullFace) {
		// Enables Cull Facing
		glEnable(GL_CULL_FACE);
		// Keeps front faces
		glCullFace(GL_FRONT);
	}
	else {
		glDisable(GL_CULL_FACE);
	}
	if (m_depth)
		// Enables the Depth Buffer
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	if (m_counter_clockwise)
		glFrontFace(GL_CCW);
	else
		glFrontFace(GL_CW);

	camera_position = { glm::vec3( camera.GetPosition()) };
	camera_rotation = { glm::vec3(camera.GetRotation()) };
	// Clear buffers from previous frame
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Compute viewport and projection matrix
	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);
	const float aspect_ratio = viewportSize[2] / (float)viewportSize[3];

	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspect_ratio, 1.0f, 4000.0f);

	// Compute camera view matrix and combine with projection matrix for passing to shader
	glm::mat4 view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());
	glm::mat4 combined_xform = projection_xform * view_xform;
	

	if (m_shadow_map)
	{
		{
			shadowMapBufferInstance.RenderShadowMapBuffer(shadow_program, light_source_position);
			// Render Light
			lightSource.RenderLight(light_source_program, camera, glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
			//lightSource.RenderPointLight(light_source_program, camera, glm::mat4(combined_xform));

			/*for (size_t i = 0; i < m_pointLightObjects.size(); ++i) {
				m_pointLightObjects[i].RenderLight(light_source_program, camera, glm::mat4(combined_xform), glm::vec3(m_pointLightPositions[i]));
			}*/

			// Render Terrain
			terrainInstance.RenderTerrain(terrain_program, camera, glm::vec3(terrain_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);


			if (m_single_light) {
				w_armyJeepInstance.RenderWildWest(west_program, camera, glm::vec3(army_jeep_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
				w_redJeepInstance.RenderWildWest(west_program, camera, glm::vec3(red_jeep_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
				w_appleInstance.RenderWildWest(west_program, camera, glm::vec3(apple_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
				w_wildWestInstance.RenderWildWest(west_program, camera, glm::vec3(wild_west_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
				w_caveInstance.RenderWildWest(west_program, camera, glm::vec3(cave_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
				w_cargoInstance.RenderWildWest(west_program, camera, glm::vec3(cargo_position), glm::mat4(combined_xform), glm::vec3(light_source_position));

			}
			else {
				// Render Objects
				armyJeepInstance.RenderObject(obj_program, camera, glm::vec3(army_jeep_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
				redJeepInstance.RenderObject(obj_program, camera, glm::vec3(red_jeep_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
				appleInstance.RenderObject(obj_program, camera, glm::vec3(apple_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
				wildWestInstance.RenderObject(obj_program, camera, glm::vec3(wild_west_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
				caveInstance.RenderObject(west_program, camera, glm::vec3(cave_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
				cargoInstance.RenderObject(west_program, camera, glm::vec3(cargo_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
			}

			shadowMapBufferInstance.RenderShadowMapBufferAfter(depth_program);

			if (m_single_light) {
				w_armyJeepInstance.RenderWildWest(west_program, camera, glm::vec3(army_jeep_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
				w_redJeepInstance.RenderWildWest(west_program, camera, glm::vec3(red_jeep_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
				w_appleInstance.RenderWildWest(west_program, camera, glm::vec3(apple_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
				w_wildWestInstance.RenderWildWest(west_program, camera, glm::vec3(wild_west_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
				w_caveInstance.RenderWildWest(west_program, camera, glm::vec3(cave_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
				w_cargoInstance.RenderWildWest(west_program, camera, glm::vec3(cargo_position), glm::mat4(combined_xform), glm::vec3(light_source_position));

			}
			else {
				// Render Objects
				armyJeepInstance.RenderObject(obj_program, camera, glm::vec3(army_jeep_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
				redJeepInstance.RenderObject(obj_program, camera, glm::vec3(red_jeep_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
				appleInstance.RenderObject(obj_program, camera, glm::vec3(apple_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
				wildWestInstance.RenderObject(obj_program, camera, glm::vec3(wild_west_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
				caveInstance.RenderObject(west_program, camera, glm::vec3(cave_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
				cargoInstance.RenderObject(west_program, camera, glm::vec3(cargo_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
			}

			//depthBufferInstance.RenderDepthBuffer(depth_program, camera);
		}
	}
	else {
		// Render Light
		lightSource.RenderLight(light_source_program, camera, glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
		//lightSource.RenderPointLight(light_source_program, camera, glm::mat4(combined_xform));

		/*for (size_t i = 0; i < m_pointLightObjects.size(); ++i) {
			m_pointLightObjects[i].RenderLight(light_source_program, camera, glm::mat4(combined_xform), glm::vec3(m_pointLightPositions[i]));
		}*/

		// Render Terrain
		terrainInstance.RenderTerrain(terrain_program, camera, glm::vec3(terrain_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);

		if (m_single_light) {
			w_armyJeepInstance.RenderWildWest(west_program, camera, glm::vec3(army_jeep_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
			w_redJeepInstance.RenderWildWest(west_program, camera, glm::vec3(red_jeep_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
			w_appleInstance.RenderWildWest(west_program, camera, glm::vec3(apple_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
			w_wildWestInstance.RenderWildWest(west_program, camera, glm::vec3(wild_west_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
			w_caveInstance.RenderWildWest(west_program, camera, glm::vec3(cave_position), glm::mat4(combined_xform), glm::vec3(light_source_position));
			w_cargoInstance.RenderWildWest(west_program, camera, glm::vec3(cargo_position), glm::mat4(combined_xform), glm::vec3(light_source_position));

		}
		else {
			// Render Objects
			armyJeepInstance.RenderObject(obj_program, camera, glm::vec3(army_jeep_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
			redJeepInstance.RenderObject(obj_program, camera, glm::vec3(red_jeep_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
			appleInstance.RenderObject(obj_program, camera, glm::vec3(apple_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
			wildWestInstance.RenderObject(obj_program, camera, glm::vec3(wild_west_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
			caveInstance.RenderObject(west_program, camera, glm::vec3(cave_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
			cargoInstance.RenderObject(west_program, camera, glm::vec3(cargo_position), glm::mat4(combined_xform), glm::vec3(light_source_position), point_light_source_position);
		}
	}
}
