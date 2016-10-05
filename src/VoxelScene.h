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

class System;

class VoxelScene
{
public:
	VoxelScene(){}
	VoxelScene(System * system);
	void UploadVoxelPointer(Voxel voxelPtr[], size_t voxelCount);
	void RenderVoxelScene(vec3 lightDir, vec3 lightColor) const;
	size_t GetVoxelCount() const { return _voxelc; }
	
	mat4 *UnlockScene(bool writeOnly=true);
	void LockScene();

	vec3 *UnlockColormap(bool writeOnly=true);
	void LockColormap();

private:
	Voxel* _voxels;
	size_t _voxelc;
	System * _sys;
	mat4 *_modelMatrices;
	vec3 *_modelColors;

	GLuint mb, cb;

public:
	static void SetPosition(mat4* _modelMatrix, glm::vec3 position);
	static void SetScale(mat4* _modelMatrix, glm::vec3 scale);
	static void SetIdentity(mat4* _modelMatrix);
	static void SetColor(vec3 *_color, glm::vec3 color);
	static void Plot(mat4* _modelMatrix, glm::vec3 position, glm::vec3 scale);
};
