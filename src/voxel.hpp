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

#pragma once

#include "defs.hpp"
#include <vector>

class Voxel
{
public:
	Voxel(){}
	Voxel(glm::vec3 position, glm::vec3 scale, glm::vec3 color);
	
	void SetPosition(glm::vec3 position);
	void SetScale(glm::vec3 scale);
	void SetIdentity();
	void SetColor(glm::vec3 color);
	void Plot(glm::vec3 position, glm::vec3 scale);

	static void RenderVoxel();

	glm::mat4 GetModelMatrix() const;
	glm::vec3 GetColor() const;

	static GLuint mVAO, mVertexBuffer, mNormalBuffer, mElementBuffer;

	static std::vector<GLfloat> voxelData;
	static std::vector<GLfloat> voxelDataNormal;
	static std::vector<unsigned int> voxelIndices;
	static void BuildVoxelData();
private:
	glm::mat4 mModelMatrix;
	glm::vec3 mColor;

	//static const unsigned int Voxel::voxelIndices[];
	//static const GLfloat voxelData[];
	
};