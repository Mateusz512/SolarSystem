#version 330

uniform int gObjectIndex; 

out vec4 FragColor;

void main()
{
    FragColor = vec4(gObjectIndex/255.0,0,0,0);
}