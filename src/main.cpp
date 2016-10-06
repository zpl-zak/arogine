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
#include "common/text2D.hpp"

// Include interns
#include "System.h"
#include <module/perlin.h>

float deltaTime = 0;
float lastTime = 0;

int main( void )
{
	System sys;
	
	int nbFrames = 0;
	double lastframeshot = glfwGetTime();

	initText2D("Holstein.dds");

	noise::module::Perlin gen;


	VoxelScene randomtest = VoxelScene(&sys);

	Voxel randomone = Voxel();
	randomone.Plot(vec3(0.f, -5.f, 0.f), vec3(.05f));
	randomone.SetColor(vec3(1, 1, 0));
	randomtest.UploadVoxelPointer(&randomone, 1);

	size_t pw=0, ph=0, pw2=0, ph2=0;
	size_t size = 0, size2=0;

	int windoww = 0, windowh = 0;
	glfwGetWindowSize(sys.GetWindow()->GetWindow(), &windoww, &windowh);

	float *pixels = VoxelImage::DownloadImage(size, "doom.png.ari", pw, ph);
	float *pixels2 = VoxelImage::DownloadImage(size2, "doom2.png.ari", pw2, ph2);

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
			float intensity = sqrtf(r*r + g*g + b*b);
			voxels[k].Plot(vec3(i, 0, j), vec3(.01f));
			voxels[k].SetColor(vec3(r, g, b));
			k++;
		}
	}
	//free(pixels);
	sys.GetVoxelScene()->UploadVoxelPointer(static_cast<Voxel*>(voxels), m);

	double fmr = .1;
	do{
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = lastTime - currentTime;
		
		nbFrames++;
		bool odd = (currentTime - lastframeshot) >= .1f;
		if (odd) { // If last prinf() was more than 1 sec ago
				   // printf and reset timer


			fmr = nbFrames;
			nbFrames = 0;
			lastframeshot += .1f;
		}

		sys.BeginFrame(deltaTime);
		{
			vec3 *col = sys.GetVoxelScene()->UnlockColormap();
			q = 0;
			for(size_t i =0;i<sys.GetVoxelScene()->GetVoxelCount();i++,q+=3)
			{
				float r = pixels[q];
				float g = pixels[q + 1];
				float b = pixels[q + 2];
				float r2 = pixels2[q];
				float g2 = pixels2[q + 1];
				float b2 = pixels2[q + 2];
				float t = abs(cos(currentTime));

				float rf = (r + t*(r2 - r));
				float gf = (g + t*(g2 - g));
				float bf = (b + t*(b2 - b));

				col[i] = vec3(rf,gf,bf);
			}
			sys.GetVoxelScene()->LockColormap();
		}
		sys.EndFrame();

		glDisable(GL_DEPTH_TEST);
		static char buf[999] = {};
		sprintf(buf, "%f ms/frame (%f FPS)\n", fmr, 1000.0f / fmr);
		printText2D(buf, 10, 10, 12);
		glEnable(GL_DEPTH_TEST);

		randomtest.RenderVoxelScene();

		sys.GetWindow()->Update();

		lastTime = currentTime;
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(sys.GetWindow()->GetWindow(), GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(sys.GetWindow()->GetWindow()) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	delete[] voxels;
	free(pixels);
	free(pixels2);

	return 0;
}

