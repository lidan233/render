#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 6) in float modelId ;

uniform mat4 model ;
uniform mat4 view ;
uniform mat4 projection ;
uniform bool isUp ;
uniform bool useUp ;


out float modelColor ;


void main()
{
    modelColor = modelId/32.0 ;
    gl_Position = projection * view * model * vec4(aPos,1) ;
    gl_Position =useUp&&isUp?vec4(0,0,0,0):gl_Position ;
}