#include "ShaderHandler.h"

ShaderHandler::ShaderHandler()
{
	_program = LoadShaders("shader/simple.vs", "shader/simple.fs");

	mmID = glGetUniformLocation(_program, "M");
	mvID = glGetUniformLocation(_program, "V");
	mpID = glGetUniformLocation(_program, "P");
	cID = glGetUniformLocation(_program, "COL");
	lnID = glGetUniformLocation(_program, "LIT");
	lcID = glGetUniformLocation(_program, "LIC");
}

GLuint ShaderHandler::GetProgram() const
{
	return _program;
}

void ShaderHandler::Publish(mat4 m, mat4 v, mat4 p, vec3 col, vec3 lit, vec3 lic) const
{
	glUseProgram(_program);
	glUniformMatrix4fv(mmID, 1, GL_FALSE, &m[0][0]);
	glUniformMatrix4fv(mvID, 1, GL_FALSE, &v[0][0]);
	glUniformMatrix4fv(mpID, 1, GL_FALSE, &p[0][0]);
	glUniform3fv(cID, 1, &col[0]);
	glUniform3fv(lnID, 1, &lit[0]);
	glUniform3fv(lcID, 1, &lic[0]);
}
