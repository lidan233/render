#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view ;

uniform bool useUp ;
uniform bool isUp ;

void main()
{
    gl_Position = view * model * vec4(aPos, 1.0);
    gl_Position =useUp&&isUp?vec4(0,0,0,0):gl_Position ;

}