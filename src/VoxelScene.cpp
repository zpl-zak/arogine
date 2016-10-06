#include "VoxelScene.h"
#include "voxel.hpp"
#include "System.h"

VoxelScene::VoxelScene(): mVoxelCount(0), mSystem(nullptr), mModelMatrices(nullptr), mModelColors(nullptr), mb(0), cb(0)
{
}

VoxelScene::VoxelScene(System *system)
{
	mSystem = system;
	
	Voxel::BuildVoxelData(); // IMPORTANT!!
	mVoxelCount = 0;

	mb = cb = 0;
	mModelMatrices = nullptr;
	mModelColors = nullptr;
}

void VoxelScene::UploadVoxelPointer(const Voxel* voxelPtr, size_t voxelCount)
{
	mVoxelCount = voxelCount;

	if (mModelMatrices)
	{
		free(mModelMatrices);
		mModelMatrices = nullptr;
	}

	if(mModelColors)
	{
		free(mModelColors);
		mModelColors = nullptr;
	}

	mModelMatrices = static_cast<mat4*>(malloc(sizeof(mat4) * voxelCount));
	mModelColors = static_cast<vec3*>(malloc(sizeof(vec3) * voxelCount));

	for(size_t i=0; i<voxelCount; i++)
	{
		mModelMatrices[i] = voxelPtr[i].GetModelMatrix();
		mModelColors[i] = voxelPtr[i].GetColor();
	}

	if(mb != 0)
	{
		glDeleteBuffers(1, &mb);
	}

	glGenBuffers(1, &mb);
	glBindBuffer(GL_ARRAY_BUFFER, mb);
	
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16 * voxelCount, mModelMatrices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, voxelCount * sizeof(mat4), &mModelMatrices[0][0][0], GL_STATIC_DRAW);

	if (cb != 0)
	{
		glDeleteBuffers(1, &cb);
	}

	glGenBuffers(1, &cb);
	glBindBuffer(GL_ARRAY_BUFFER, cb);
	glBufferData(GL_ARRAY_BUFFER, voxelCount * sizeof(vec3), &mModelColors[0][0], GL_STATIC_DRAW);
}

void VoxelScene::RenderVoxelScene() const
{
	if (mb == 0 || cb == 0)return;

	mSystem->GetShaderHandler()->Publish(
		mSystem->GetCamera()->GetViewMatrix(),
		mSystem->GetCamera()->GetProjectionMatrix(),
		vec3(1.f), vec3(1.f)
	);

	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Voxel::mVertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));
	//glVertexAttribDivisor(0, 1);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, Voxel::mNormalBuffer);
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Voxel::mElementBuffer);

	glDrawElementsInstanced(GL_TRIANGLES, Voxel::voxelIndices.size(), GL_UNSIGNED_INT, static_cast<void*>(nullptr), mVoxelCount);
	
	glDisableVertexAttribArray(6);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

mat4* VoxelScene::UnlockScene(bool writeOnly) const
{
	glBindBuffer(GL_ARRAY_BUFFER, mb);
	auto r = static_cast<mat4*>(glMapBuffer(GL_ARRAY_BUFFER, (writeOnly) ? GL_WRITE_ONLY : GL_READ_WRITE));
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

vec3* VoxelScene::UnlockColormap(bool writeOnly) const
{
	//glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, cb);
	auto r = static_cast<vec3*>(glMapBuffer(GL_ARRAY_BUFFER, (writeOnly) ? GL_WRITE_ONLY : GL_READ_WRITE));
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

void VoxelScene::SetPosition(mat4* mModelMatrix, glm::vec3 position)
{
	*mModelMatrix *= translate(position);
}

void VoxelScene::SetScale(mat4* mModelMatrix, glm::vec3 scale)
{
	*mModelMatrix *= glm::scale(scale);
}

void VoxelScene::SetIdentity(mat4* mModelMatrix)
{
	*mModelMatrix = mat4(1.f);
}

void VoxelScene::SetColor(vec3 *_color, glm::vec3 color)
{
	*_color = color;
}

void VoxelScene::Plot(mat4* mModelMatrix, glm::vec3 position, glm::vec3 scale)
{
	SetIdentity(mModelMatrix);
	*mModelMatrix = translate(position * (scale * vec3(2.f))) * glm::scale(scale);
}