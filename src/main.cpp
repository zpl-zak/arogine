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


int main(void)
{
	System sys;

	auto nbFrames = 0;
	auto lastframeshot = glfwGetTime();

	initText2D("Holstein.dds");

	noise::module::Perlin gen;


	auto randomtest = VoxelChunk(sys.GetCamera());
	auto mainscene = VoxelChunk(sys.GetCamera());
	//auto raym = RaymarchChunk(sys.GetCamera());

	size_t pw = 0, ph = 0, pw2 = 0, ph2 = 0;
	size_t size = 100, size2 = 0;

	auto windoww = 0, windowh = 0;
	glfwGetWindowSize(sys.GetWindow()->GetWindow(), &windoww, &windowh);

//	auto pixels = VoxelImage::DownloadImage(size, "doom.png.ari", pw, ph);
//	auto pixels2 = VoxelImage::DownloadImage(size2, "doom2.png.ari", pw2, ph2);

	noise::module::Perlin pn;

	pw = ph = 768;

	Camera::speed = 50;

	float sea_level = 0.f;
	size_t k = 0, q = 0, l = 0, cnt = 0;
	auto m = size;
	{
		std::vector<Voxel> voxels;
		for (unsigned i = 0; i < pw; i++)
		{
			for (unsigned j = 0; j < ph; j++)
			{
				for (unsigned l = 0; l < 1; l++, q += 3)
				{
					float rnd = rand() % 10;
					Voxel v;
					//				//auto intensity = sqrtf(r * r + g * g + b * b);
					auto col = pn.GetValue(i / 100.f, l / 100.f, j / 100.f) * 10;
					col = round(col);
					if (col < 3.f)
					{
						v.SetColor(vec3(0, 0, 1));
						col = 2;
					}
					else if (col > 8.f)
					{
						v.SetColor(vec3(.8, .8, .8));
					}
					else
					{
						v.SetColor(vec3(0.05*rnd, 1, 0));
					}
					k++;
					float x = i;// (i > 32) ? i + col : (i == 32) ? i : i - col;
					float y = l;// (j > 32) ? j + col : (j == 32) ? j : j - col;
					float z = j;// (l > 32) ? l + col : (l == 32) ? l : l - col;


					
					v.Plot(vec3(x, y-col, z), vec3(1.f));
					voxels.push_back(v);
					
					if (k > 32)
					{
						mainscene.UploadVoxels(voxels, cnt, pw*ph);
						cnt += voxels.size();
						voxels.clear();
						k = 0;
					}
				}
			}
		}
		
	}
	
	auto fmr = .1;
	do
	{
		auto currentTime = static_cast<float>(glfwGetTime());
		deltaTime = lastTime - currentTime;

		nbFrames++;
		if (currentTime - lastframeshot >= 1.f)
		{
			fmr = nbFrames;
			nbFrames = 0;
			lastframeshot += 1.f;
		}

		sys.BeginFrame(deltaTime);
		/*{
			auto col = mainscene.UnlockColormap();
			q = 0;
			for (size_t i = 0; i < mainscene.GetVoxelCount(); i++ , q += 3)
			{
				auto r = pixels.at(q);
				auto g = pixels.at(q + 1);
				auto b = pixels.at(q + 2);
				auto r2 = pixels2.at(q);
				auto g2 = pixels2.at(q + 1);
				auto b2 = pixels2.at(q + 2);
				auto t = abs(cos(currentTime));

				auto rf = (r + t * (r2 - r));
				auto gf = (g + t * (g2 - g));
				auto bf = (b + t * (b2 - b));

				col[i] = vec3(rf, gf, bf);
			}
			mainscene.LockColormap();
		}*/
		mainscene.Render();
		//raym.Render();

		glDisable(GL_DEPTH_TEST);
		static char buf[999] = {};
		sprintf(buf, "%f ms/frame (%f FPS)\n", 1000.0f / fmr, fmr);
		printText2D(buf, 10, 10, 12);
		glEnable(GL_DEPTH_TEST);

		sys.EndFrame();

		lastTime = currentTime;
	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(sys.GetWindow()->GetWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(sys.GetWindow()->GetWindow()) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

