#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;                                   
layout (location = 3) in mat4 instanceModel;            

out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 normalMatrix;

void main()
{
	if(gl_InstanceID != 0){		
		gl_Position = projection * view * model * instanceModel * vec4(aPos, 1.0);
	}else{
		gl_Position = projection * view * model * vec4(aPos, 1.0);	
	}
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = (normalMatrix * vec4(aNormal,0.0)).xyz;
    vs_out.TexCoords = aTexCoords;
}