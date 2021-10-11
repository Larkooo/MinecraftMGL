#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

out vec3 FragColor;

void main()
{
	FragColor = aNormal;
	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0f);
}