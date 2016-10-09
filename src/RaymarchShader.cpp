#include "RaymarchShader.h"

RaymarchShader::RaymarchShader()
{
	mProgram = LoadShaders("shader/raymarch.vs", "shader/raymarch.fs");

	//mmID = glGetUniformLocation(mProgram, "M");
	mvID = glGetUniformLocation(mProgram, "V");
	mpID = glGetUniformLocation(mProgram, "P");
	lnID = glGetUniformLocation(mProgram, "LIT");
	lcID = glGetUniformLocation(mProgram, "LIC");
}

void RaymarchShader::Publish(mat4 v, mat4 p, vec3 lit, vec3 lic) const
{
	glUseProgram(mProgram);
	glUniformMatrix4fv(mvID, 1, GL_FALSE, &v[0][0]);
	glUniformMatrix4fv(mpID, 1, GL_FALSE, &p[0][0]);
	glUniform3fv(lnID, 1, &lit[0]);
	glUniform3fv(lcID, 1, &lic[0]);
}
