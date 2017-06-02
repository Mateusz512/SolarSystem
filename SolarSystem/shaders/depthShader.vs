#version 330 core
layout (location = 0) in vec3 aPos;                 
layout (location = 3) in mat4 instanceModel;            

uniform mat4 model;
uniform mat4 normalMatrix;

void main()
{
    //gl_Position = model * vec4(aPos, 1.0);
	if(gl_InstanceID != 0){		
		gl_Position = instanceModel * vec4(aPos, 1.0);
	}else{
		gl_Position = model * vec4(aPos, 1.0);	
	}
}