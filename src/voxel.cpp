// /*
//  Copyright 2016 AROGAN Group
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//  http://www.apache.org/licenses/LICENSE-2.0
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// */

#include "voxel.hpp"

Voxel::Voxel(glm::vec3 position, glm::vec3 scale, glm::vec3 color)
{
	SetPosition(position);
	SetScale(scale);
	SetColor(color);
}

void Voxel::SetPosition(glm::vec3 position)
{
	_modelMatrix *= translate(position);
}

void Voxel::SetScale(glm::vec3 scale)
{
	_modelMatrix *= glm::scale(scale);
}

void Voxel::SetIdentity()
{
	_modelMatrix = mat4(1.f);
}

void Voxel::SetColor(glm::vec3 color)
{
	_color = color;
}

void Voxel::RenderVoxel()
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Voxel::vb);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Voxel::nb);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);

	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
	//glDrawElements(GL_TRIANGLES, sizeof(unsigned int) * 36, GL_UNSIGNED_INT, static_cast<void*>(nullptr));

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}


glm::mat4 Voxel::GetModelMatrix() const
{
	return _modelMatrix;
}

glm::vec3 Voxel::GetColor() const
{
	return _color;
}

GLuint Voxel::vao = 0;
GLuint Voxel::vb = 0;
GLuint Voxel::nb = 0;
GLuint Voxel::eb = 0;

const unsigned int Voxel::voxelIndices[] = {
	0,  1,  2,  0,  2,  3,   //front
	4,  5,  6,  4,  6,  7,   //right
	8,  9,  10, 8,  10, 11,  //back
	12, 13, 14, 12, 14, 15,  //left
	16, 17, 18, 16, 18, 19,  //upper
	20, 21, 22, 20, 22, 23 }; //bottom

const GLfloat Voxel::voxelData[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

void Voxel::BuildVoxelData()
{
	if (vao == 0 || vb == 0)
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vb);
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 108, voxelData, GL_STATIC_DRAW);

		glGenBuffers(1, &eb);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 36, voxelIndices, GL_STATIC_DRAW);

		glGenBuffers(1, &nb);
		glBindBuffer(GL_ARRAY_BUFFER, nb);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 108, voxelData, GL_STATIC_DRAW);
		printf("Voxel data has been sent to the GPU!\n");
	}
}
