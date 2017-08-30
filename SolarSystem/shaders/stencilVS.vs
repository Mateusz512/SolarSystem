#version 330 core
layout (location = 0) in vec3 aPos;                             
layout (location = 3) in mat4 instanceModel;             

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	if(gl_InstanceID != 0){		
		gl_Position = projection * view * model * instanceModel * vec4(aPos, 1.0);
	}else{
		gl_Position = projection * view * model * vec4(aPos, 1.0);	
	}
}