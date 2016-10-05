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

// Include commons
#include "defs.hpp"
#include "common/shader.hpp"
#include "common/VoxelImage.h"

// Include interns
#include "System.h"

float deltaTime = 0;
float lastTime = 0;

int main( void )
{
	System sys;
	
	int nbFrames = 0;
	double lastframeshot = glfwGetTime();

	size_t pw=0, ph=0;
	size_t size = 0;

	float *pixels = VoxelImage::DownloadImage(size, "test.png.ari", pw, ph);

	size_t m = size;
	Voxel *voxels = new Voxel[m];
	size_t k = 0,q=0;
	for(size_t i = 0; i<pw;i++)
	{
		for(size_t j = 0; j<ph;j++, q += 3)
		{
			float r = pixels[q];
			float g = pixels[q + 1];
			float b = pixels[q + 2];
			voxels[k].Plot(vec3(i, .0f, j), vec3(.1f));
			voxels[k].SetColor(vec3(r, g, b));
			k++;
		}
	}
	free(pixels);
	sys.GetVoxelScene()->UploadVoxelPointer((Voxel*)voxels, m);

	do{
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = lastTime - currentTime;
		

		nbFrames++;
		if (currentTime - lastframeshot >= 1.0) { // If last prinf() was more than 1 sec ago
											 // printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastframeshot += 1.0;
		}

		vec3 lightVec(1, 1, 1);
		vec3 lightColor(1, 1, 1);

		sys.BeginFrame(deltaTime);
		{

		}
		sys.EndFrame(lightVec, lightColor);

		lastTime = currentTime;
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(sys.GetWindow()->GetWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(sys.GetWindow()->GetWindow()) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	delete[] voxels;

	return 0;
}

