#version 330 core
#define LIGHT_COUNT 5


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 5) in float curvature ;
layout (location = 6) in float modelId ; //节点所在的牙齿（model)的id 便于选择牙齿

out vec3 WorldPos; // 节点的时间坐标
out vec3 vNormal; //view 空间的法向量
out float gradientFactor ; // 正弦函数变黄的程度
out float Curvature ; //曲率
out vec3 vView ; // view空间中 从节点指向相机的向量
out vec3 vLightDir[LIGHT_COUNT] ; // view 空间中 从节点指向光源的向量
out vec3 ViewSpacePos ;// view 空间中 的节点向量
out float modelChoose ; // 对应model的ID

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform bool useUp ;// 是否显示上面的牙齿
uniform bool isUp ;// 是否是上面的牙齿
uniform vec3 boundmin ; // minx,miny,minz
uniform vec3 boundmax ;// maxx maxy maxz

uniform vec3 camPos;//相机的世界坐标


uniform vec4 lightPos[LIGHT_COUNT]; // 光源的坐标 view空间


void main()
{
	modelChoose = modelId ;

    WorldPos = vec3(model * vec4(aPos, 1.0));
    ViewSpacePos = (view * model *vec4(aPos, 1.0)).xyz ;

    vNormal = vec3(view*model*vec4(aNormal,0.0)) ;
    vView =  vec3(view*vec4(camPos,1.0)) - ViewSpacePos ;

    for(int i =0 ;i < LIGHT_COUNT;i++)
    {
        vLightDir[i] = lightPos[i].xyz - ViewSpacePos ;
    }


    gl_Position =  projection * view * vec4(WorldPos, 1.0);
    gl_Position =useUp&&isUp?vec4(0,0,0,0):gl_Position ;

    gradientFactor = isUp?sin(1.57*clamp((boundmax.z - aPos.z)/(boundmax.z-boundmin.z),0.0,1.0))
                    :sin(1.57*clamp(( aPos.z - boundmin.z)/(boundmax.z-boundmin.z),0.0,1.0)) ;

    // 提振一下Curvature
    Curvature = curvature*0.5+0.5 ;

}