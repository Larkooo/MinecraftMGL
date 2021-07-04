#version 330 core

in vec3 FragPos;

void main()
{
	gl_FragColor = vec4(gl_FragCoord.x / 800, gl_FragCoord.y / 600, 1.0f, 1.0f);
}