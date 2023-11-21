#pragma once
#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"

class VBO
{

public:
	GLuint ID;

	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();

	void Unbind();

	void Delete();
};

