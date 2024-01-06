#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"


class Shader
{
protected:

	GLuint Program;

public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void Use()
};

