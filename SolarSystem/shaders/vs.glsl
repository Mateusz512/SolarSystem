#version 330

uniform mat4 projectionMatrix;
uniform mat4 model;
uniform mat4 ViewMatrix;
uniform mat4 normalMatrix;

layout (location = 0) in vec3 inPosition;
//layout (location = 1) in vec3 inColor;
layout (location = 1) in vec3 inTexCoord;
layout (location = 2) in vec3 inNormal;

out vec3 vNormal;
smooth out vec4 kolorek;

out vec2 texCoord;

void main()
{	
	gl_Position = projectionMatrix*ViewMatrix*model*vec4(inPosition, 1.0);
	vec4 vRes = normalMatrix*vec4(inNormal, 0.0);
	vNormal = vRes.xyz;
	kolorek = vec4(0.9,0.9,0.9,1.0);
	texCoord = vec2(inTexCoord[0],inTexCoord[1]);	
}