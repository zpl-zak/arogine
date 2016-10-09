#version 330 core

layout(location = 0) in vec3 vert;
layout(location = 1) in vec2 uv;

//uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 LIT;
uniform vec3 LIC;

out vec2 uvcoord;

void main() {
	gl_Position = vec4(vert,1);
	uvcoord = uv;
}