#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aOffset;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

vec3 scale = vec3(1.0f, 1.0f, 1.0f);

out vec3 FragPos;

void main()
{
	// scale our model matrix to the desired size
	mat4 model = uModel;
	model[0][0] *= scale.x;
	model[1][1] *= scale.y;
	model[2][2] *= scale.z;

	FragPos = vec3(model * vec4((aPos + aOffset) * scale, 1.0f));
	gl_Position = uProj * uView * model * vec4((aPos + aOffset) * scale, 1.0f);
}