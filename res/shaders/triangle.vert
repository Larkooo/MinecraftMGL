#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec2 aTile;
layout (location = 4) in vec2 aFactor;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

vec3 scale = vec3(1.0f, 1.0f, 1.0f);

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec2 Tile;
out vec2 Factor;

void main()
{
	Normal = aNormal;
	TexCoords = aTex;
	Tile = aTile;
	Factor = aFactor;
	FragPos = vec3(uModel * vec4(aPos, 1.0f));
	gl_Position = uProj * uView * uModel * vec4(aPos, 1.0f);
}