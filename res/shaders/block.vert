#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;
layout (location = 3) in mat4 aModel;
// 3 vec2 containing our tiles positions
layout (location = 7) in mat3x2 aTile;

uniform mat4 uProj;
uniform mat4 uView;

vec3 scale = vec3(1.0f, 1.0f, 1.0f);

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out mat3x2 Tile;

void main()
{
	FragPos = vec3(aModel * vec4(aPos, 1.0f));
	Normal = aNormal;
	TexCoords = aTex;
	Tile = aTile;
	gl_Position = uProj * uView * aModel * vec4(aPos, 1.0f);
}