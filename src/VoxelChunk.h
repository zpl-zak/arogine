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
#include "voxel.hpp"
#include "Camera.h"
#include "BaseShader.h"

class System;

/**
 * \class VoxelScene
 * \brief Batch voxel renderer
 * 
 * This class handles uploading an array of pre-defined voxels to the GPU.
 * It also takes care of updating our shader program with updated uniforms.
 * Renderer binds buffers and calls instanced draw on GPU.
 * There are also static method helpers for manipulating unlocked voxels.
 */
class VoxelChunk
{
public:
	explicit VoxelChunk(Camera* camera);

	/** 
	 * \brief Upload voxel array to the GPU.
	 * \param voxelvec Pointer to an array of voxels.
	 *  
	 * Method generates new buffers and uploads a sequence of voxel properties to the GPU.
	 */
	void UploadVoxels(const std::vector<Voxel>& voxelvec, int offset, int totalSize);

	/** 
	 * \brief Invoke render commands.
	 *  
	 * Method publishes updated uniforms to the shader program and maps buffers to the shader inputs.
	 * It then invokes render command, which produces instanced meshes.
	 */
	void Render() const;

	auto GetVoxelCount() const -> size_t const { return mVoxelCount; }

	/**
	 * \brief Unlocks an access to the uploaded voxel array properties.
	 * \param writeOnly (Optional) Decides whether to ask the GPU for copying video memory to system, therefore allowing us to read data from GPU.
	 * 
	 * Calls map buffer command which returns a pointer to a memory region representing our uploaded data.
	 */
	mat4 *UnlockScene(bool writeOnly=true) const;
	static void LockScene();

	/**
	* \brief Unlocks an access to the uploaded voxel color properties.
	* \param writeOnly (Optional) Decides whether to ask the GPU for copying video memory to system, therefore allowing us to read data from GPU.
	*
	* Calls map buffer command which returns a pointer to a memory region representing our uploaded data.
	*/
	vec3 *UnlockColormap(bool writeOnly=true) const;
	static void LockColormap();

private:
	size_t mVoxelCount;
	Camera * mCamera;
	std::vector<mat4> mModelMatrices;
	std::vector<vec3> mModelColors;

	GLuint mb, cb;
	BaseShader mBaseShader;

public:
	static void SetPosition(mat4* mModelMatrix, glm::vec3 position);
	static void SetScale(mat4* mModelMatrix, glm::vec3 scale);
	static void SetIdentity(mat4* mModelMatrix);
	static void SetColor(vec3 *_color, glm::vec3 color);
	static void Plot(mat4* mModelMatrix, glm::vec3 position, glm::vec3 scale);
};
