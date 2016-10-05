#version 330 core

layout(location = 0) in vec3 vert;
layout(location = 1) in vec3 norm;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 COL;
uniform vec3 LIT;
uniform vec3 LIC;

out vec3 voxcolor;
out vec3 normal;
out vec3 eyedir;
out vec3 light;
out vec3 lightcolor;

void main() {
	gl_Position = P * V * M * vec4(vert,1);

	eyedir = (vec3(0,0,0) - (V * M * vec4(vert,1)).xyz).xyz;
	light = (V * vec4(LIT,1)).xyz + eyedir;
	normal = (V * M * vec4(norm,1)).xyz;

	voxcolor = COL;
	lightcolor = LIC;
}