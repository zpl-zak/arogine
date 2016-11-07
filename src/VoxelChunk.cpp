#include "VoxelChunk.h"
#include "voxel.hpp"
#include "System.h"

VoxelChunk::VoxelChunk(Camera* camera) : mVoxelCount(0)
{
	mCamera = camera;
	
	Voxel::BuildVoxelData(); // IMPORTANT!!

	mb = cb = 0;
}

void VoxelChunk::UploadVoxels(const std::vector<Voxel>& voxelvec, int offset = 0, int totalSize = 0)
{
	if (mModelMatrices.size() > 0)
	{
		mModelMatrices.clear();
	}

	if(mModelColors.size() > 0)
	{
		mModelColors.clear();
	}

	for(size_t i=0; i<voxelvec.size(); i++)
	{
		mModelMatrices.push_back(voxelvec.at(i).GetModelMatrix());
		mModelColors.push_back(voxelvec.at(i).GetColor());
	}

	if(mb == 0)
	{
		glGenBuffers(1, &mb);
		glBindBuffer(GL_ARRAY_BUFFER, mb);
		glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(mat4), nullptr, GL_STATIC_DRAW);
		mVoxelCount = totalSize;
	}

	{
		glBindBuffer(GL_ARRAY_BUFFER, mb);
		glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(mat4), voxelvec.size() * sizeof(mat4), &mModelMatrices[0][0][0]);
	}

	if (cb == 0)
	{
		glGenBuffers(1, &cb);
		glBindBuffer(GL_ARRAY_BUFFER, cb);
		glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(vec3), nullptr, GL_STATIC_DRAW);
	}

	{
		glBindBuffer(GL_ARRAY_BUFFER, cb);
		glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(vec3), voxelvec.size() * sizeof(vec3), &mModelColors[0][0]);
	}

	mModelMatrices.clear();
	mModelMatrices.shrink_to_fit();
	mModelColors.clear();
	mModelColors.shrink_to_fit();
}

void VoxelChunk::Render() const
{
	if (mb == 0 || cb == 0)return;

	mBaseShader.Publish(
		mCamera->GetViewMatrix(),
		mCamera->GetProjectionMatrix()
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

mat4* VoxelChunk::UnlockScene(bool writeOnly) const
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

void VoxelChunk::LockScene()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

vec3* VoxelChunk::UnlockColormap(bool writeOnly) const
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

void VoxelChunk::LockColormap()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void VoxelChunk::SetPosition(mat4* mModelMatrix, glm::vec3 position)
{
	*mModelMatrix *= translate(position);
}

void VoxelChunk::SetScale(mat4* mModelMatrix, glm::vec3 scale)
{
	*mModelMatrix *= glm::scale(scale);
}

void VoxelChunk::SetIdentity(mat4* mModelMatrix)
{
	*mModelMatrix = mat4(1.f);
}

void VoxelChunk::SetColor(vec3 *_color, glm::vec3 color)
{
	*_color = color;
}

void VoxelChunk::Plot(mat4* mModelMatrix, glm::vec3 position, glm::vec3 scale)
{
	SetIdentity(mModelMatrix);
	*mModelMatrix = translate(position * (scale * vec3(2.f))) * glm::scale(scale);
}