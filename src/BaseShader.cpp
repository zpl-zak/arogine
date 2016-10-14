#include "BaseShader.h"

BaseShader::BaseShader()
{
	mProgram = LoadShaders("shader/simple.vs", "shader/simple.fs");

	mViewMatrixID = glGetUniformLocation(mProgram, "V");
	mProjectionMatrix = glGetUniformLocation(mProgram, "P");
}

auto BaseShader::GetProgram() const -> GLuint
{
	return mProgram;
}

void BaseShader::Publish(mat4 v, mat4 p) const
{
	glUseProgram(mProgram);
	glUniformMatrix4fv(mViewMatrixID, 1, GL_FALSE, &v[0][0]);
	glUniformMatrix4fv(mProjectionMatrix, 1, GL_FALSE, &p[0][0]);
}
