#version 330 core

in vec3 voxcolor;
in vec3 normal;
in vec3 eyedir;
in vec3 light;
in vec3 lightcolor;

out vec3 color;

void main() {
	vec3 n = normalize(normal);
	vec3 l = normalize(light);

	float cosT = clamp(dot(n,l),0,1);

	vec3 ambient = vec3(0.1,0.1,0.1) * voxcolor;
	color = ambient + voxcolor * lightcolor;
}