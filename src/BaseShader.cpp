#include "BaseShader.h"

BaseShader::BaseShader()
{
	mProgram = LoadShaders("shader/simple.vs", "shader/simple.fs");

	mmID = glGetUniformLocation(mProgram, "M");
	mvID = glGetUniformLocation(mProgram, "V");
	mpID = glGetUniformLocation(mProgram, "P");
	lnID = glGetUniformLocation(mProgram, "LIT");
	lcID = glGetUniformLocation(mProgram, "LIC");
}

auto BaseShader::GetProgram() const -> GLuint
{
	return mProgram;
}

void BaseShader::Publish(mat4 v, mat4 p, vec3 lit, vec3 lic) const
{
	glUseProgram(mProgram);
	glUniformMatrix4fv(mvID, 1, GL_FALSE, &v[0][0]);
	glUniformMatrix4fv(mpID, 1, GL_FALSE, &p[0][0]);
	glUniform3fv(lnID, 1, &lit[0]);
	glUniform3fv(lcID, 1, &lic[0]);
}
