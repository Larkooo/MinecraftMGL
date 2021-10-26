#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec2 aTile;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec2 Tile;

void main()
{
	FragPos = vec3(uModel * vec4(aPos, 1.0f));

	Normal = aNormal;
	TexCoords = aTex;
	Tile = aTile;
	
	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0f);
}