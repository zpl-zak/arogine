#include "System.h"

System::System()
{
	mCamera = Camera(vec3(0, 0, 0), vec2(3.14f, 0.f));
	mVoxelScene = VoxelScene(this);
}
void System::BeginFrame(float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mCamera.HandleInput(mWindow.GetWindow(), deltaTime);
}

void System::EndFrame() const
{
	mVoxelScene.RenderVoxelScene();
}
