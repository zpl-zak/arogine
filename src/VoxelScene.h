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

private:
	Voxel* _voxels;
	size_t _voxelc;
	System * _sys;
};
