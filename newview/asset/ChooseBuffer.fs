#version 330 core

in float modelColor ;
out vec4 FragColor ;

void main()
{
    FragColor = vec4(modelColor,modelColor,modelColor,1) ;
}