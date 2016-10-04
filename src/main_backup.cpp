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
#include "voxel.hpp"
#include "Camera.h"
#include "LinearMath/btVector3.h"

GLFWwindow* window;

float deltaTime = 0;
float lastTime = 0;

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Arogine", NULL, NULL);
	if( window == nullptr ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. We are sorry for that :/.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	Voxel::BuildVoxelData(); // IMPORTANT!!

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	GLuint programID = LoadShaders("shader/simple.vs", "shader/simple.fs");

	Camera cam(vec3(0, 0, 0), vec2(3.14f, 0.f));

	size_t voxw = 30;
	size_t voxh = 30;
	size_t voxd = 30;

	Voxel*** voxels = new Voxel**[voxw];
	for(size_t i=0; i<voxw;i++)
	{
		voxels[i] = new Voxel*[voxh];
		for(size_t j=0; j<voxw;j++)
		{
			voxels[i][j] = new Voxel[voxd];
			for(size_t k=0; k<voxd;k++)
			{
				voxels[i][j][k].Plot(vec3(i, k, j), vec3(.01f, .01f, .01f));
				voxels[i][j][k].SetColor(vec3(i, j, k));
			}
		}
	}

	GLuint mmID = glGetUniformLocation(programID, "M");
	GLuint mvID = glGetUniformLocation(programID, "V");
	GLuint mpID = glGetUniformLocation(programID, "P");
	GLuint cID = glGetUniformLocation(programID, "COL");
	GLuint lnID = glGetUniformLocation(programID, "LIT");
	GLuint lcID = glGetUniformLocation(programID, "LIC");

	int nbFrames = 0;
	double lastframeshot = glfwGetTime();

	do{
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = lastTime - currentTime;
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		nbFrames++;
		if (currentTime - lastframeshot >= 1.0) { // If last prinf() was more than 1 sec ago
											 // printf and reset timer
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastframeshot += 1.0;
		}

		cam.HandleInput(window, deltaTime);

		vec3 lightVec(1, 1, 1);
		vec3 lightColor(1, 1, 1);

		mat4 mv = cam.GetProjectionMatrix() * cam.GetViewMatrix();

		size_t Idx = 0;
		for(size_t i = 0; i < voxw; i++)
		{
			for(size_t j=0; j<voxh;j++)
			{
				float angle = (currentTime)*20.f;
				float s = sin(radians(angle));
				float c = cos(radians(angle));
				float x = i;
				float y = j;
				float o = 49.f;
				//float im = (sin(abs(sin(currentTime))*i/2.f)+cos(currentTime))/10.f;
				float im = 0.f;// x*x + 0.3f * x + 0.12f;
				float xpos = c * (x - o) - s * (y - o) + o;
				float ypos = s * (x - o) + c * (y - o) + o;
				float x2 = xpos - o;
				float y2 = ypos - o;

				/* sphere code
				if (sqrtf(x2*x2 + y2*y2) >= 45.000099994f * .02f)
					continue;

				
				if (Idx % 2 == 0)
					im = sqrtf(x2*x2 + y2*y2)*sqrtf(x2*x2 + y2*y2);
				else
					im = -sqrtf(x2*x2 + y2*y2)*sqrtf(x2*x2 + y2*y2) + 80.f*.02f;*/

				im = (Idx % voxw);

				if(Idx % 2 == 0)
				{
					im = -im;
				}

				for(size_t k=0; k<voxd;k++)
				{
					//voxels[i][j][k].Plot(vec3(i, k, j), vec3(.01f, .01f, .01f));

					voxels[i][j][k].SetColor(vec3(abs(cos(currentTime)), abs(sin(currentTime)), abs(tan(currentTime))));

					mat4 modelMatrix = voxels[i][j][k].GetModelMatrix();// * glm::rotate(angle, vec3(1,0,0));
					vec3 modelColor = voxels[i][j][k].GetColor();

					//mat4 mvp = mv * modelMatrix;

					glUseProgram(programID);
					glUniformMatrix4fv(mmID, 1, GL_FALSE, &modelMatrix[0][0]);
					glUniformMatrix4fv(mvID, 1, GL_FALSE, &cam.GetViewMatrix()[0][0]);
					glUniformMatrix4fv(mpID, 1, GL_FALSE, &cam.GetProjectionMatrix()[0][0]);
					//glUniformMatrix4fv(mID, 1, GL_FALSE, &mvp[0][0]);
					glUniform3fv(cID, 1, &modelColor[0]);
					glUniform3fv(lnID, 1, &lightVec[0]);
					glUniform3fv(lcID, 1, &lightColor[0]);

					Voxel::RenderVoxel();
					Idx++;
				}
			}
		}
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		lastTime = currentTime;
	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

