#include "VoxelScene.h"
#include "voxel.hpp"
#include "System.h"

VoxelScene::VoxelScene(System *system)
{
	_sys = system;
	
	Voxel::BuildVoxelData(); // IMPORTANT!!
	_voxels = nullptr;
	_voxelc = 0;

	mb = cb = 0;
	_modelMatrices = nullptr;
	_modelColors = nullptr;
}

void VoxelScene::UploadVoxelPointer(Voxel voxelPtr[], size_t voxelCount)
{
	_voxels = voxelPtr;
	_voxelc = voxelCount;

	if (_modelMatrices)
	{
		free(_modelMatrices);
		_modelMatrices = nullptr;
	}

	if(_modelColors)
	{
		free(_modelColors);
		_modelColors = nullptr;
	}

	_modelMatrices = static_cast<mat4*>(malloc(sizeof(mat4) * voxelCount));
	_modelColors = static_cast<vec3*>(malloc(sizeof(vec3) * voxelCount));

	for(size_t i=0; i<voxelCount; i++)
	{
		_modelMatrices[i] = voxelPtr[i].GetModelMatrix();
		_modelColors[i] = voxelPtr[i].GetColor();
	}

	if(mb != 0)
	{
		glDeleteBuffers(1, &mb);
	}

	glGenBuffers(1, &mb);
	glBindBuffer(GL_ARRAY_BUFFER, mb);
	
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16 * voxelCount, _modelMatrices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, voxelCount * sizeof(mat4), &_modelMatrices[0][0][0], GL_STATIC_DRAW);

	if (cb != 0)
	{
		glDeleteBuffers(1, &cb);
	}

	glGenBuffers(1, &cb);
	glBindBuffer(GL_ARRAY_BUFFER, cb);
	glBufferData(GL_ARRAY_BUFFER, voxelCount * sizeof(vec3), &_modelColors[0][0], GL_STATIC_DRAW);
}

void VoxelScene::RenderVoxelScene(vec3 lightDir, vec3 lightColor) const
{
	if (mb == 0 || cb == 0)return;

	_sys->GetShaderHandler()->Publish(
		_sys->GetCamera()->GetViewMatrix(),
		_sys->GetCamera()->GetProjectionMatrix(),
		lightDir, lightColor
	);

	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Voxel::vb);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));
	//glVertexAttribDivisor(0, 1);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Voxel::nb);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));
	//glVertexAttribDivisor(1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, mb);
	for(size_t i =0; i < 4; i++)
	{
		glEnableVertexAttribArray(2+i);
		glVertexAttribPointer(2+i, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), reinterpret_cast<void*>(sizeof(vec4) * i));
		glVertexAttribDivisor(2+i, 1);
	}
	
	glEnableVertexAttribArray(6);
	glBindBuffer(GL_ARRAY_BUFFER, cb);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));
	glVertexAttribDivisor(6, 1);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 12 * 3, _voxelc);
	
	glDisableVertexAttribArray(6);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

mat4* VoxelScene::UnlockScene(bool writeOnly)
{
	glBindBuffer(GL_ARRAY_BUFFER, mb);
	auto r = (mat4*)glMapBuffer(GL_ARRAY_BUFFER, (writeOnly) ? GL_WRITE_ONLY : GL_READ_WRITE);
	if (!r)
	{
		printf("glMap error: %d\n", glGetError());
		getchar();
		exit(-1);
	}
	return r;
}

void VoxelScene::LockScene()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

vec3* VoxelScene::UnlockColormap(bool writeOnly)
{
	//glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, cb);
	auto r = (vec3*)glMapBuffer(GL_ARRAY_BUFFER, (writeOnly) ? GL_WRITE_ONLY : GL_READ_WRITE);
	if(!r)
	{
		printf("glMap error: %d\n", glGetError());
		getchar();
		exit(-1);
	}
	return r;
}

void VoxelScene::LockColormap()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void VoxelScene::SetPosition(mat4* _modelMatrix, glm::vec3 position)
{
	*_modelMatrix *= translate(position);
}

void VoxelScene::SetScale(mat4* _modelMatrix, glm::vec3 scale)
{
	*_modelMatrix *= glm::scale(scale);
}

void VoxelScene::SetIdentity(mat4* _modelMatrix)
{
	*_modelMatrix = mat4(1.f);
}

void VoxelScene::SetColor(vec3 *_color, glm::vec3 color)
{
	*_color = color;
}

void VoxelScene::Plot(mat4* _modelMatrix, glm::vec3 position, glm::vec3 scale)
{
	SetIdentity(_modelMatrix);
	*_modelMatrix = translate(position * (scale * vec3(2.f))) * glm::scale(scale);
}