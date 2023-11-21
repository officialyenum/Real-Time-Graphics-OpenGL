#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset)
{
	VBO.Bind();
	glEnableVertexAttribArray(layout);
	glVertexAttribPointer(
		layout,                  // attribute 0
		numComponents,                  // size in bytes of each item in the stream
		type,           // type of the item
		GL_FALSE,           // normalized or not (advanced)
		stride,                  // stride (advanced)
		offset            // array buffer offset (advanced)
	);
	VBO.Unbind();

}

void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
}
