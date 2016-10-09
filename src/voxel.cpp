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
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing fla

Voxel::Voxel(glm::vec3 position, glm::vec3 scale, glm::vec3 color)
{
	SetPosition(position);
	SetScale(scale);
	SetColor(color);
}

void Voxel::SetPosition(glm::vec3 position)
{
	mModelMatrix *= translate(position);
}

void Voxel::SetScale(glm::vec3 scale)
{
	mModelMatrix *= glm::scale(scale);
}

void Voxel::SetIdentity()
{
	mModelMatrix = mat4(1.f);
}

void Voxel::SetColor(glm::vec3 color)
{
	mColor = color;
}

void Voxel::Plot(glm::vec3 position, glm::vec3 scale)
{
	SetIdentity();
	mModelMatrix = translate(position * (scale * vec3(2.f))) * glm::scale(scale);
}

void Voxel::RenderVoxel()
{
	
}

auto Voxel::GetColor() const -> vec3 const
{
	return mColor;
}

auto Voxel::GetModelMatrix() const -> mat4 const
{
	return mModelMatrix;
}

GLuint Voxel::mVAO = 0;
GLuint Voxel::mVertexBuffer = 0;
GLuint Voxel::mNormalBuffer = 0;
GLuint Voxel::mElementBuffer = 0;

std::vector<GLfloat> Voxel::voxelData;
std::vector<GLfloat> Voxel::voxelDataNormal;
std::vector<unsigned int> Voxel::voxelIndices;


void Voxel::BuildVoxelData()
{
	if (mVAO == 0 || mVertexBuffer == 0)
	{
		Assimp::Importer Importer;
		const aiScene* scene = Importer.ReadFile("voxel.obj", aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

		if(!scene)
		{
			printf("Could not load voxel.obj!\n");
			getchar();
			exit(-1);
		}

		const aiMesh* mesh = scene->mMeshes[0];

		for(size_t i=0; i<mesh->mNumVertices; i++)
		{
			const aiVector3D* pos = &(mesh->mVertices[i]);
			const aiVector3D* normal = &(mesh->mNormals[i]);

			voxelData.push_back(pos->x);
			voxelData.push_back(pos->y);
			voxelData.push_back(pos->z);

			voxelDataNormal.push_back(normal->x);
			voxelDataNormal.push_back(normal->y);
			voxelDataNormal.push_back(normal->z);
		}

		for(size_t i=0;i<mesh->mNumFaces;i++)
		{
			const aiFace& face = mesh->mFaces[i];

			voxelIndices.push_back(face.mIndices[0]);
			voxelIndices.push_back(face.mIndices[1]);
			voxelIndices.push_back(face.mIndices[2]);
		}

		glGenVertexArrays(1, &mVAO);
		glBindVertexArray(mVAO);

		glGenBuffers(1, &mVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * voxelData.size(), &voxelData[0], GL_STATIC_DRAW);

		glGenBuffers(1, &mElementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * voxelIndices.size(), &voxelIndices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &mNormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * voxelData.size(), &voxelData[0], GL_STATIC_DRAW);
		printf("Voxel data has been sent to the GPU!\n");
	}
}
