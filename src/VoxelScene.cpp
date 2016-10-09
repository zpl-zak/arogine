#include "VoxelScene.h"
#include "voxel.hpp"
#include "System.h"

VoxelScene::VoxelScene(Camera* camera)
{
	mCamera = camera;
	
	Voxel::BuildVoxelData(); // IMPORTANT!!
	mVoxelCount = 0;

	mb = cb = 0;
}

void VoxelScene::UploadVoxels(const std::vector<Voxel> voxelvec, size_t voxelCount)
{
	mVoxelCount = voxelCount;

	if (mModelMatrices.size() > 0)
	{
		mModelMatrices.clear();
	}

	if(mModelColors.size() > 0)
	{
		mModelColors.clear();
	}

	for(size_t i=0; i<voxelCount; i++)
	{
		mModelMatrices.push_back(voxelvec.at(i).GetModelMatrix());
		mModelColors.push_back(voxelvec.at(i).GetColor());
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

	mModelMatrices.clear();
	mModelColors.clear();
}

void VoxelScene::Render() const
{
	if (mb == 0 || cb == 0)return;

	mBaseShader.Publish(
		mCamera->GetViewMatrix(),
		mCamera->GetProjectionMatrix(),
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