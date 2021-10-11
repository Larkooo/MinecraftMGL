#version 330 core

in vec3 FragColor;

void main()
{
	gl_FragColor = vec4(FragColor, 1.0f);
}