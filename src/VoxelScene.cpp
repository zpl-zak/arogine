#include "VoxelScene.h"
#include "voxel.hpp"
#include "System.h"

VoxelScene::VoxelScene(System *system)
{
	_sys = system;
	
	Voxel::BuildVoxelData(); // IMPORTANT!!
	_voxels = nullptr;
	_voxelc = 0;
}

void VoxelScene::UploadVoxelPointer(Voxel voxelPtr[], size_t voxelCount)
{
	_voxels = voxelPtr;
	_voxelc = voxelCount;
}

void VoxelScene::RenderVoxelScene(vec3 lightDir, vec3 lightColor) const
{
	for(size_t i=0; i<_voxelc; i++)
	{
		_sys->GetShaderHandler()->Publish(
			_voxels[i].GetModelMatrix(),
			_sys->GetCamera()->GetViewMatrix(),
			_sys->GetCamera()->GetProjectionMatrix(),
			_voxels[i].GetColor(),
			lightDir, lightColor

		);

		Voxel::RenderVoxel();
	}
}
