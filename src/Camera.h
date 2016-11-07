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

#include "defs.hpp"

class Camera
{
public:
	Camera(){}
	Camera(glm::vec3 position, glm::vec2 angle);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	void SetPosition(glm::vec3 position);
	void SetAngle(glm::vec2 angle);
	void HandleInput(GLFWwindow* window, float deltaTime);

	static float speed, mouseSpeed;
private:
	glm::vec3 mPosition;
	glm::vec2 mAngle;
	glm::mat4 mViewMatrix, mProjMatrix;

	static const glm::vec3 upVector;
};
