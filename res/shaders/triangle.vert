#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 3) in mat4 aInstanceModel;

uniform mat4 uProj;
uniform mat4 uView;

out vec3 FragPos;

void main()
{
	FragPos = vec3(aInstanceModel * vec4(aPos, 1.0f));
	gl_Position = uProj * uView * aInstanceModel * vec4(aPos, 1.0f);
}