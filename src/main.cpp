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

// Include interns
#include "System.h"

float deltaTime = 0;
float lastTime = 0;

int main( void )
{
	System sys;
	
	int nbFrames = 0;
	double lastframeshot = glfwGetTime();

	size_t m = 1000;
	Voxel *voxels = new Voxel[m];
	for(size_t k = 0; k<10; k++)
	{
		for(size_t j = 0; j < 10;j++)
		{
			for(size_t i=0;i<10; i++)
			{
				voxels[i + 10 * (j + 10 * k)].Plot(vec3(i, k, j), vec3(.1f));
				voxels[i + 10 * (j + 10 * k)].SetColor(vec3(k, j, i));
			}
		}
	}
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
			size_t Idx = 0;
			for (size_t k = 0; k<10; k++)
			{

				for (size_t j = 0; j < 10; j++)
				{
					float angle = (currentTime)*20.f;
					float s = sin(radians(angle));
					float c = cos(radians(angle));
					float x = j;
					float y = k;
					float o = 49.f;
					//float im = (sin(abs(sin(currentTime))*i/2.f)+cos(currentTime))/10.f;
					float im = 0.f;// x*x + 0.3f * x + 0.12f;
					float xpos = c * (x - o) - s * (y - o) + o;
					float ypos = s * (x - o) + c * (y - o) + o;
					float x2 = xpos - o;
					float y2 = ypos - o;
					for (size_t i = 0; i<1; i++)
					{
						
						voxels[i + 10 * (j + 10 * k)].Plot(vec3(xpos, k, ypos), vec3(.01f));
						voxels[i + 10 * (j + 10 * k)].SetColor(vec3(k, j, i));
					}
					Idx++;
				}

			}
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

