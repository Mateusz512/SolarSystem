#version 330

layout (location = 0) in vec3 aPos;                         
layout (location = 3) in mat4 instanceModel;  

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);	
}