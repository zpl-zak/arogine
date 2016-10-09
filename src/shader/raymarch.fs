#version 330 core

in vec2 uvcoord;

out vec3 color;

void main() {
	color = vec3(uvcoord,1.0f);
}