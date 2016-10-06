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

#include "Window.h"
#include "Camera.h"
#include "ShaderHandler.h"
#include "VoxelScene.h"

class System
{
public:
	System();

	void BeginFrame(float deltaTime);
	void EndFrame() const;

	Window *GetWindow() { return &mWindow; }
	VoxelScene *GetVoxelScene() { return &mVoxelScene; }
	ShaderHandler *GetShaderHandler() { return &mShaderHandler; }
	Camera *GetCamera() { return &mCamera; }

private:
	Window mWindow;
	Camera mCamera;
	ShaderHandler mShaderHandler;
	VoxelScene mVoxelScene;
};
