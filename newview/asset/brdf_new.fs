#version 330 core
#define LIGHT_COUNT 5
#define SHADOW_LIGHT_COUNT 5

uniform vec3 camPos; // 相机位置
uniform bool isUp ; //牙齿是否是位于上面 在这个文件中主要用于给上下槽牙提供不同的光照 可删除
uniform float roughness  ; // 牙齿粗糙程度
uniform float reflection ;//牙齿反射系数
uniform vec3 mat_ambient_color ;//环境光color
uniform vec2 cursorPos ; //光标位置
uniform bool chooseTooth; // 是否根据光标位置选择对应的牙齿
uniform vec4 shadowLightPos[SHADOW_LIGHT_COUNT] ; // 阴影位置 因为阴影和光照是不匹配的 有阴影的地方未必有光照
uniform float far_plane ;// 在我的代码中 光锥和视锥的远平面均设定为同一值 far_plane


//后期可以重构为结构体 但是我为了方便调整 就没有重构
in vec3 vNormal;
in vec3 vView;
in vec3 vLightDir[LIGHT_COUNT];
in float gradientFactor;
in float Curvature ;
in vec3 WorldPos;
in vec3 ViewSpacePos ;
in float modelChoose ;

vec3 lights[3*LIGHT_COUNT];
const int AMBIENT 	 = 0;
const int DIFFUSE 	 = 1;
const int SPECULAR 	 = 2;


const vec3 mat_diffuse_color   = vec3(.45, .45, .45);
const vec3 mat_specular_color  = vec3(.6, .6, .6); 
const vec3 light_ambient_color  = vec3(0.7529411911964417, 0.7529411911964417, 0.7529411911964417); 


const float opacity = 1;

//纹理列表
uniform samplerCube texture_shadow_cube1;
uniform samplerCube texture_shadow_cube2;
uniform samplerCube texture_shadow_cube3;
uniform sampler2D texture_choose_buffer1 ;



vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

//鼠标所指向的modelID是否和当前渲染模型的modelID相等呢？如果相等就被选中了
bool isFragColor()
{
    float t = texture(texture_choose_buffer1,vec2(cursorPos.x,1-cursorPos.y)).r*32;
    return t >= 1 && t== modelChoose?true:false;
}

//为了对抗锯齿和波动 以求更加平滑的阴影所以选择PCF方法计算阴影
float ShadowCalculatePCF(vec3 fragPos,int i,samplerCube texture1)
{
    vec3 fragToLight = fragPos - shadowLightPos[i].xyz ;
    float currentDepth = length(fragToLight) ;

    float shadow = 0.0;

    //float bias = 0.005*tan(acos(dot(normalize(vNormal),normalize(vLightDir[i])))); // cosTheta is dot( n,l ), clamped between 0 and 1
    //bias = clamp(bias, 0,0.01);
     float bias = 1;
    int samples = 20;
    float viewDistance = length(camPos - fragPos);

    float diskRadius = (1.0 + (viewDistance / far_plane)) /4;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(texture1, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= far_plane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow ;
}

float ShadowCalculationAllPCF(vec3 fragPos)
{

    float shadow1 = ShadowCalculatePCF(fragPos,0,texture_shadow_cube1) ;
    float shadow2 = ShadowCalculatePCF(fragPos,1,texture_shadow_cube2) ;
    float shadow3 = ShadowCalculatePCF(fragPos,2,texture_shadow_cube3) ;

    //shadow1*0.25+shadow2*0.25+shadow3*0.25+shadow4*0.25+shadow5*0.1
    //return isUp?(shadow4*0.2+0.8):(shadow5*0.2+0.8);
    //return shadow5;
    //return (shadow4 +shadow5)*0.2+0.7 ;
    return ((1-shadow1)*0.2+0.8);
}



void main(void)
{
    //我们的光源坐标是固定在view空间的 我们的skybox记录的也是view空间中的点距离光源的距离
    //所以我们根据view空间的坐标去确定是不是被遮挡
    float shadow = ShadowCalculationAllPCF(ViewSpacePos) ;

    //以下为张斌学长代码
	vec3 fragColor = vec3(0.0); 
	vec3 texColor = mix( vec3(1.0,0.92,0.84), vec3(0.94,0.94,0.94), gradientFactor ); 
	lights[AMBIENT]  = vec3( 1, 1, 1 ); 
	lights[DIFFUSE]  = vec3( 0.4588, 0.4549, 0.4470); 
	lights[SPECULAR] = vec3( 0.0784, 0.0784, 0.0784);
	lights[3 + AMBIENT]  = vec3(1, 1, 1);
	lights[3 + DIFFUSE]  = vec3( 0.4588, 0.4549, 0.4470);
	lights[3 + SPECULAR] = vec3( 0.05882353,  0.05882353,  0.05882353 );
	lights[6 + AMBIENT]  = vec3(1, 1, 1);
	lights[6 + DIFFUSE]  = vec3( 0.4588, 0.4549, 0.4470);
	lights[6 + SPECULAR] = vec3( 0.05882353,  0.05882353,  0.05882353 );
	lights[9 + AMBIENT]  = vec3(1, 1, 1);
	lights[9 + DIFFUSE]  = vec3( 0.4588, 0.4549, 0.4470);
	lights[9 + SPECULAR] = vec3( 0.05882353,  0.05882353,  0.05882353 );
	lights[12 + AMBIENT]  = vec3(1, 1, 1);
    lights[12 + DIFFUSE]  = vec3( 0.4588, 0.4549, 0.4470);
    lights[12 + SPECULAR] = vec3( 0.05882353,  0.05882353,  0.05882353 );

	for(int i = 0; i < LIGHT_COUNT; i += 1 ) 
	{ 
		vec3 source_direction = normalize(vLightDir[i]); 
		vec3 source_diffuse_color  	= lights[i * 3 + DIFFUSE]; 
		vec3 source_specular_color 	= lights[i * 3 + SPECULAR]; 

		vec3 V = normalize(vView); 
		vec3 N = normalize(vNormal); 
		vec3 halfVector = normalize(V + source_direction); 
		float NdotL = dot(N, source_direction); 
		float NdotH = max(0.0, dot(N, halfVector)); 
		float NdotV = max(0.0, dot(N, V)); 
		float VdotH = max(0.0, dot(V, halfVector)); 
		if(NdotL < 0.0){ 
			NdotL = 0.0; 
		} 
		// roughness n 
		float D = 0.0; 
		if(NdotH != 0.0){ 
			float r1 = 1.0 / (3.14 * roughness * roughness * pow(NdotH, 4.0)); 
			float r2 = (NdotH * NdotH - 1.0) / (roughness * roughness * NdotH * NdotH); 
			D = r1 * exp(r2); 
		} 
		// geometric attenuation n 
		float NH2 = 2.0 * NdotH; 
		float G = 0.0; 
		if(VdotH != 0.0 ){ 
			float g1 = (NH2 * NdotV) / VdotH; 
			float g2 = (NH2 * NdotL) / VdotH; 
			G  = min(1.0, min(g1, g2)); 
		} 
		//fresnel n 
		// Schlick approximation n 
		float F = pow(1.0 - VdotH, 5.0); 
		F *= (1.0 - reflection); 
		F += reflection; 
		float ct = 0.0; 
		if(NdotV * NdotL != 0.0){ 
			ct = (F * G * D) / (NdotV * NdotL * 3.14); 
		} 
		ct = D * reflection; 
		//vec3 R = reflect(source_direction, N); n 
		//ct = pow(max(dot(R,V),0.), 100.0); n
		vec3 I_diffuse  = source_diffuse_color * mat_diffuse_color * texColor * NdotL; 
		vec3 I_specular = source_specular_color * mat_specular_color * ct; 
		vec3 I = (I_diffuse + I_specular*shadow);
		fragColor += I;
	}

    //我们在计算环境光的时候 加入曲率的影响 这是曲率导致的自遮挡
	fragColor +=  mat_ambient_color * light_ambient_color * texColor*(Curvature*0.9+0.1);

	//加入阴影的影响
	vec3 color = mix(fragColor*shadow, gl_Color.xyz , gl_Color.w) ;

	//在最后的光照中 再次加入曲率的影响
	color = mix(vec3(0.8,0.72,0.64),color,(Curvature*0.7+0.3)) ;


    //选中牙齿 如果能够选中而且选中 则颜色和蓝色点乘
    vec3 color1 = isFragColor()?(color*vec3(0.5,0.5,0.95)):color ;
	gl_FragColor = vec4(vec3(color1), opacity);

} 
