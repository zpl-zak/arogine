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

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec2 angle)
{
	SetPosition(position);
	SetAngle(angle);
}

glm::mat4 Camera::GetViewMatrix() const
{
	return _viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return _projMatrix;
}

void Camera::SetPosition(glm::vec3 position)
{
	_position = position;
}

void Camera::SetAngle(glm::vec2 angle)
{
	_angle = angle;
}

void Camera::HandleInput(GLFWwindow* window, float deltaTime)
{
	//TODO(zaklaus): Move into Input class!
	static bool firstMousePos = true;
	double xpos, ypos;
	int w, h;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwGetWindowSize(window, &w, &h);
	glfwSetCursorPos(window, w / 2, h / 2);

	if(firstMousePos)
	{
		xpos = w / 2;
		ypos = h / 2;
		firstMousePos = false;
	}

	_angle[0] += mouseSpeed * deltaTime * float(w / 2 - xpos);
	_angle[1] -= mouseSpeed * deltaTime * float(h / 2 - ypos);

	if (_angle[1] < 21.f)
		_angle[1] = 21.f;
	else if (_angle[1] > 23.f)
		_angle[1] = 23.f;

	glm::vec3 direction (
		cos(_angle[1]) * sin(_angle[0]),
		sin(_angle[1]),
		cos(_angle[1]) * cos(_angle[0])
	);

	glm::vec3 right(
		sin(_angle[0] - 3.14f / 2.f),
		0,
		cos(_angle[0] - 3.14f / 2.f)
	);

	glm::vec3 up = glm::cross(right, direction);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		_position -= direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		_position += direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		_position -= right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		_position += right * deltaTime * speed;
	}

	_projMatrix = glm::perspective(glm::radians(45.f),
		static_cast<float>(w) / static_cast<float>(h), 0.01f, 100.f);

	_viewMatrix = glm::lookAt(
		_position,
		_position+direction,
		up
	);
}


glm::vec3 const Camera::upVector(0, 1, 0);
float Camera::speed(3.f);
float Camera::mouseSpeed(.5f);