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
	// Create a new program (returns a unqiue id)
	m_program = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Shaders/core_vertex_shader.vert") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Shaders/core_fragment_shader.frag") };
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

	// Create a new program (returns a unqiue id)
	terrain_program = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Shaders/terrain_vertex_shader.vert") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Shaders/terrain_fragment_shader.frag") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(terrain_program, vertex_shader);

	// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(terrain_program, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(terrain_program))
		return false;

	return true;
}

void Renderer::CreateTerrain(int size)
{


	int numCellsX{ 3 };
	int numCellsZ{ 3 };

	int numVertsX = numCellsX + 1;
	int numVertsZ = numCellsZ + 1;

	int numTrisX = numCellsX * 2;
	int numTrisZ = numCellsZ;

	float cellSizeX = size / (float)numCellsX;
	float cellSizeZ = size / (float)numCellsZ;

	glm::vec3 startPos{ -size / 2.0f, 0, size / 2.0f};

	std::vector<glm::vec3> vertices;
	for (int z = 0; z < numVertsZ; z++)
	{
		for (int x = 0; x < numVertsX; x++)
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

	this->DrawImage(terrain_tex_, "Data\\Textures\\floor.jpg");


}

GLuint Renderer::DrawImage(GLuint textureID,const char* filename)
{

	// Todo: Load Image Texture
	Helpers::ImageLoader imageLoader;

	imageLoader.Load(filename);
	glGenTextures(1, &textureID);

	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageLoader.Width(), imageLoader.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, imageLoader.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry()
{
	// Load and compile shaders into m_program
	if (!CreateProgram())
		return false;

	std::cout <<  "---------------------------------" << "\n";
	Helpers::ModelLoader loader;
	if (!loader.LoadFromFile("Data\\Models\\Jeep\\jeep.obj"))
		return false;

	// Now we can loop through all the mesh in the loaded model:
	for (const Helpers::Mesh& mesh : loader.GetMeshVector())
	{
		MeshStruct mymesh;
		std::cout << mesh.name << " + MatIndex: " << mesh.materialIndex << "\n";

		mymesh.numElements = mesh.elements.size();

		/*
			Create Vertex Buffer Object (VBO) to hold vertex positions
		*/

		GLuint positionsVBO;
		glGenBuffers(1, &positionsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		GLuint coloursVBO;
		glGenBuffers(1, &coloursVBO);
		glBindBuffer(GL_ARRAY_BUFFER, coloursVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.normals.size(), mesh.normals.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint texcoordsVBO;
		glGenBuffers(1, &texcoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, texcoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.uvCoords.size(), mesh.uvCoords.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);



		GLuint elementsEBO;
		glGenBuffers(1, &elementsEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.elements.size(), mesh.elements.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		// VAO

		glGenVertexArrays(1, &mymesh.VAO);
		glBindVertexArray(mymesh.VAO);

	
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

		m_meshVector.push_back(mymesh);

	}
	std::cout << "---------------------------------" << "\n";



	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);

	//FXAA Framebuffer + Texture
	glGenFramebuffers(1, &fxaa_fbo_);

	this->DrawImage(fxaa_tex_, "Data\\Models\\Jeep\\jeep_rood.jpg");

	// FXAA BUFFER

	glBindFramebuffer(GL_FRAMEBUFFER, fxaa_fbo_);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fxaa_tex_, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, terrain_tex_, 0);

	GLenum bufs4[] = { 
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
	};
	glDrawBuffers(1, bufs4);

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind

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

	
	 
	
	// Compute viewport and projection matrix
	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);
	const float aspect_ratio = viewportSize[2] / (float)viewportSize[3];
	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspect_ratio, 1.0f, 4000.0f);



	// Compute camera view matrix and combine with projection matrix for passing to shader
	glm::mat4 view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());
	glm::mat4 combined_xform = projection_xform * view_xform;
	glm::mat4 model_xform = glm::mat4(1);
	// Draw Terrain
	glUseProgram(terrain_program);

	// Send the combined matrix to the shader in a uniform
	glUniform1f(glGetUniformLocation(terrain_program, "gMinHeight"), 2.0f);
	glUniform1f(glGetUniformLocation(terrain_program, "gMaxHeight"), 2.0f);
	glUniformMatrix4fv(glGetUniformLocation(terrain_program, "model_xform"), 1, GL_FALSE, glm::value_ptr(model_xform));
	

	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, terrain_tex_);
	glUniform1i(glGetUniformLocation(terrain_program, "terrain_tex"), 0);


	// Draw Jeep
	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(m_program);

	// Send the combined matrix to the shader in a uniform
	glUniformMatrix4fv(glGetUniformLocation(m_program, "combined_xform"), 1, GL_FALSE, glm::value_ptr(combined_xform));


	// Uncomment all the lines below to rotate cube first round y then round x
	//static float angle = 0;
	//static bool rotateY = true;

	/*angle += 0.001f;
	if (angle > glm::two_pi<float>())
	{
		angle = 0;
		rotateY = !rotateY;
	}*/

	// Send the model matrix to the shader in a uniform
	glUniformMatrix4fv(glGetUniformLocation(m_program, "model_xform"), 1, GL_FALSE, glm::value_ptr(model_xform));


	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, fxaa_tex_);
	glUniform1i(glGetUniformLocation(m_program, "sample_tex"), 0);

	// Bind our VAO and render



	for (const MeshStruct mesh : m_meshVector)
	{
		glBindVertexArray(mesh.VAO);
		glDrawElements(GL_TRIANGLES, mesh.numElements, GL_UNSIGNED_INT, (void*)0);
	}


	//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glBindFramebuffer(GL_READ_FRAMEBUFFER, fxaa_fbo_);
	//glBlitFramebuffer(0, 0, viewportSize[2], viewportSize[3], 0, 0, viewportSize[2], viewportSize[3], GL_COLOR_BUFFER_BIT, GL_LINEAR);



}
