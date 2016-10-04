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

	Voxel voxels[100][100];
	for(size_t i=0; i<100;i++)
	{
		for(size_t j=0; j<100;j++)
		{
			voxels[i][j].SetPosition(vec3(i*2, 0, j*2));
			voxels[i][j].SetColor(vec3(i, j, 0));
		}
	}

	GLuint mmID = glGetUniformLocation(programID, "M");
	GLuint mvID = glGetUniformLocation(programID, "V");
	GLuint mpID = glGetUniformLocation(programID, "P");
	GLuint mID = glGetUniformLocation(programID, "MVP");
	GLuint cID = glGetUniformLocation(programID, "COL");
	GLuint lnID = glGetUniformLocation(programID, "LIT");
	GLuint lcID = glGetUniformLocation(programID, "LIC");

	do{
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = lastTime - currentTime;
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		cam.HandleInput(window, deltaTime);

		vec3 lightVec(1, 1, 1);
		vec3 lightColor(1, 1, 1);

		mat4 mv = cam.GetProjectionMatrix() * cam.GetViewMatrix();

		for(size_t i = 0; i < 100; i++)
		{
			for(size_t j=0; j<100;j++)
			{
				mat4 modelMatrix = voxels[i][j].GetModelMatrix();
				vec3 modelColor = voxels[i][j].GetColor();

				mat4 mvp = mv * modelMatrix;

				glUseProgram(programID);
				glUniformMatrix4fv(mmID, 1, GL_FALSE, &modelMatrix[0][0]);
				glUniformMatrix4fv(mvID, 1, GL_FALSE, &cam.GetViewMatrix()[0][0]);
				glUniformMatrix4fv(mpID, 1, GL_FALSE, &cam.GetProjectionMatrix()[0][0]);
				glUniformMatrix4fv(mID, 1, GL_FALSE, &mvp[0][0]);
				glUniform3fv(cID, 1, &modelColor[0]);
				glUniform3fv(lnID, 1, &lightVec[0]);
				glUniform3fv(lcID, 1, &lightColor[0]);

				glEnableVertexAttribArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, Voxel::vb);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));

				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, Voxel::nb);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(nullptr));

				glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(0);
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

