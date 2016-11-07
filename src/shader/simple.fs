#version 330 core
#extension GL_EXT_gpu_shader4 : require

in vec3 voxcolor;
 in vec3 normal;
in vec3 eyedir;
in vec3 light;
in vec3 lightcolor;
in vec3 pos;

out vec4 color;

void main() {
	vec3 n = normalize(normal);
	vec3 lp = vec3(1,1,1);
	vec3 l = normalize(light);

	float distance = length(lp - pos);

	float cosT = clamp(dot(n,-l),0,1);

	vec3 ambient = vec3(0.8,0.8,0.8) * voxcolor;
	color = vec4(ambient + voxcolor * cosT, .5);// / (distance*distance) ;
	//color = vec3((n.xyz + vec3(1)) / 2);
}