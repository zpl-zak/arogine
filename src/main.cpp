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

	Voxel*** voxels = new Voxel**[100];
	for(size_t i=0; i<100;i++)
	{
		voxels[i] = new Voxel*[100];
		for(size_t j=0; j<100;j++)
		{
			voxels[i][j] = new Voxel[100];
			for(size_t k=0; k<100;k++)
			{
				voxels[i][j][k].SetPosition(vec3(i * .02f, k * .02f, j * .02f));
				voxels[i][j][k].SetScale(vec3(.01f,.01f,.01f));
				voxels[i][j][k].SetColor(vec3(i, j, k));
			}
		}
	}

	GLuint mmID = glGetUniformLocation(programID, "M");
	GLuint mvID = glGetUniformLocation(programID, "V");
	GLuint mpID = glGetUniformLocation(programID, "P");
	GLuint mID = glGetUniformLocation(programID, "MVP");
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

		for(size_t i = 0; i < 100; i++)
		{
			for(size_t j=0; j<100;j++)
			{
				//float im = (sin(abs(sin(currentTime))*i/2.f)+cos(currentTime))/10.f;
				float im = cos(i*j);
				float angle = (currentTime)*20.f;
				for(size_t k=0; k<1;k++)
				{
					float s = sin(radians(angle));
					float c = cos(radians(angle));
					float x = i*.02f;
					float y = j*.02f;
					float o = 49.f*.02f;

					float xpos = c * (x - o) - s * (y - o) + o;
					float ypos = s * (x - o) + c * (y - o) + o;

					voxels[i][j][k].SetIdentity();
					voxels[i][j][k].SetPosition(vec3(xpos, 0.f, ypos));
					voxels[i][j][k].SetScale(vec3(.01f, .01f, .01f));

					voxels[i][j][k].SetColor(vec3(abs(cos(i)), abs(sin(i)), abs(tan(i))));

					mat4 modelMatrix = voxels[i][j][k].GetModelMatrix();// * glm::rotate(angle, vec3(1,0,0));
					vec3 modelColor = voxels[i][j][k].GetColor();

					mat4 mvp = mv * modelMatrix;

					glUseProgram(programID);
					glUniformMatrix4fv(mmID, 1, GL_FALSE, &modelMatrix[0][0]);
					glUniformMatrix4fv(mvID, 1, GL_FALSE, &cam.GetViewMatrix()[0][0]);
					glUniformMatrix4fv(mpID, 1, GL_FALSE, &cam.GetProjectionMatrix()[0][0]);
					glUniformMatrix4fv(mID, 1, GL_FALSE, &mvp[0][0]);
					glUniform3fv(cID, 1, &modelColor[0]);
					glUniform3fv(lnID, 1, &lightVec[0]);
					glUniform3fv(lcID, 1, &lightColor[0]);

					Voxel::RenderVoxel();
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

