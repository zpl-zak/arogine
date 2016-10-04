#include "System.h"

System::System()
{
	_camera = Camera(vec3(0, 0, 0), vec2(3.14f, 0.f));
	_voxelScene = VoxelScene(this);
}
void System::BeginFrame(float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_camera.HandleInput(_window.GetWindow(), deltaTime);
}

void System::EndFrame(vec3 lightDir, vec3 lightColor)
{
	
	_voxelScene.RenderVoxelScene(lightDir, lightColor);
	_window.Update();
}
