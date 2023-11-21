#pragma once
#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"



class Shader
{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();
};
#endif // !SHADER_CLASS_H
