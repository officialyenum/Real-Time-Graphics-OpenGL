#pragma once
#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"

class EBO
{

public:
	GLuint ID;

	EBO(GLuint* indices, GLsizeiptr size);

	void Bind();

	void Unbind();

	void Delete();
};

