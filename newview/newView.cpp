//
// Created by MQ on 2020/8/21.
//

#ifndef _OPENGL_NEW_VIEW_
#define _OPENGL_NEW_VIeW_



#include <glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CheckShader.h"
#include "NewCamera.h"
#include "Model.h"
#include "newView.h"
#include "WriteImage.h"


#include "IteratorForLight.h"
#include <iostream>
#include <random>
#include <math.h>


#define RENDER_IMAGE_ALL 12500
#define RENDER_IMAGE_SHADOW 10000


// 爆搜参数所需要的变量

// 目前的输出图片的数目
int currentname = 0 ;

// 默认步长数目
int floataddnum= 10 ;

// rough 参数迭代器和reflection的
Value_Iterator<float> rough(0.05f,0.25f,floataddnum) ;
Value_Iterator<float> reflection(0.05,0.45,floataddnum) ;

//颜色迭代器
glm::vec3 begin = glm::vec3(.805, .759, .736) ;//rgb开始
glm::vec3 bodong = glm::vec3(5,0.16,0.2) ;// hsv 波动范围
int addnum = 5 ;//hsv每个维度的波动step步长个数
Light_Iterator<glm::vec3> lightIterator(begin,bodong,addnum) ;

// 当前的rough 当前的reflection 当前的rgb
float currentrough ;
float currentreflection ;
glm::vec3 currentrgbt ;


//进行 rough reflection rgb的联合爆搜
void setdata()
{
    static unsigned int i = 0 ;
    unsigned int t = i ;

    currentrough = rough.get(t /(floataddnum*addnum*addnum*addnum) );
    t -= t /(floataddnum*addnum*addnum*addnum) * (floataddnum*addnum*addnum*addnum);

    currentreflection = reflection.get(t/(addnum*addnum*addnum)) ;
    t -= t/(addnum*addnum*addnum)*(addnum*addnum*addnum) ;

    currentrgbt = lightIterator.get(t) ;
    std::cout<<"currentTough: "<<currentrough
             <<"currentReflection: "<<currentreflection
             <<"currentrgb: "<<glm::to_string(currentrgbt)<<std::endl ;
    i++ ;
}

//关闭rough等爆搜时候之后的 状态设定
void setSelectdata()
{
    currentrough =0.05;
    currentreflection = 0.05;
    currentrgbt = glm::vec3(0.735,0.702,0.709);
}

// 光源位置的爆搜 以圆为单位
void setLightPos();

// 从默认缓冲中导出color buffer 用于 rough reflection rgb 联合爆搜
void saveImage();

// 从默认缓冲中导出color buffer 用于 光源位置爆搜
void saveShadowImage();


int main()
{
    // 设定要爆搜的参数的初始状态 方便转换爆搜和非爆搜渲染状态
    setSelectdata() ;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //开启深度测试
    glEnable(GL_DEPTH_TEST);

    //开启blend
    glEnable(GL_BLEND) ;
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA) ;


    // build and compile shaders
    // -------------------------
    //渲染shader
    CShader shader("..\\newview\\asset\\brdf.vs",
                   "..\\newview\\asset\\brdf_new.fs");
    //点光源shader
    CShader simpleDepthShader("..\\newview\\asset\\dianyinying.vs",
                              "..\\newview\\asset\\dianyinying.fs",
                              "..\\newview\\asset\\dianyinying.gs");
    //modelID buffer
    CShader chooseShader("..\\newview\\asset\\ChooseBuffer.vs",
                         "..\\newview\\asset\\ChooseBuffer.fs") ;

    // 放大shader 用于透明设定
    CShader scaleShader("..\\newview\\asset\\transparent.vs",
                        "..\\newview\\asset\\transparent.fs") ;

    // generate 阴影buffer
    genShadowBuffer() ;
    // generate modelID buffer
    genChooseBuffer() ;


    std::vector<Model> models  ;
    Bound* allbound = addModels(models,true) ;
    //我的曲率计算只支持三角形 如果是其他形状 可能会出错 所以牙龈不计算曲率
    std::vector<Model> yayins ;
    Bound* yayin = addModelsYayin(yayins,false) ;

    // render的时候 models和yayin是分离开的 但是当计算阴影的时候 他们要合并 所以给出一个合并的
    std::vector<Model> allmodel  ;
    allmodel.insert(allmodel.end(),models.begin(),models.end()) ;
    allmodel.insert(allmodel.end(),yayins.begin(),yayins.end()) ;


    // 设定model的纹理
    setShadowBuffer(models);
    setChooseBuffer(models) ;


    //声明opengl context中纹理内存中的bind位置
    shader.use() ;
    shader.setInt("texture_shadow_cube1",0) ;
    shader.setInt("texture_shadow_cube2",1) ;
    shader.setInt("texture_shadow_cube3",2) ;
    shader.setInt("texture_choose_buffer1",3) ;
    shader.setInt("texture_shadow1",4) ;



    while (!glfwWindowShouldClose(window)&&currentname<RENDER_IMAGE_SHADOW)
    {

//        setdata() ; // 方便爆搜光照参数
//        setLightPos() ;//方便爆搜光源位置
        showFPS(window,SCR_WIDTH,SCR_HEIGHT) ;

        process_Input(window,&camera) ;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model1 = glm::mat4(1.0f);
        model1 = glm::translate(model1,allbound->Move2Zero()) ;
        camera.setCenter(allbound->getMiddle()) ;

        glm::mat4 view = camera.GetViewMatrix() ;
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);


        //點光源的pass 只pass depth
        simpleDepthShader.use() ;
        useShadowBuffer(simpleDepthShader,allmodel,model1,view ,*allbound) ;

        // chooseBuffer的pass pass colorbuffer（modelID)
        chooseShader.use() ;
        useChooseBuffer(chooseShader,models,model1,view,projection,*allbound) ;



        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.3,0.2,0.1,1) ;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model1);
        shader.setVec3("camPos", camera.Position);
        shader.setFloat("roughness", currentrough);
        shader.setFloat("reflection", currentreflection);
        shader.setVec3("mat_ambient_color",currentrgbt) ;
        shader.setFloat("far_plane",far_plane) ;
        shader.setVec2("cursorPos",glm::vec2(cursorPosx,cursorPosy)) ;
        shader.setBool("chooseTooth",choose) ;


        for(int i  =0 ;i <LIGHT_SIZE ;i++)
        {
            shader.setVec4("lightPos[" + std::to_string(i) + "]", lightPositions[i]);
            shader.setVec4("shadowLightPos[" + std::to_string(i) + "]", ShadowLightPositions[i]);
       }
        renderTooth(shader,models,*allbound) ;
        renderTooth(shader,yayins,*allbound) ;



//        saveShadowImage() ;//是否最后爆搜导出图片

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


// 光源位置的爆搜 以圆为单位
void setLightPos()
{
    static float theta = -M_PI ,gamma = -M_PI ;
    float radius = 160 ;
    float z = radius * sin(theta) ;
    float y = radius * cos(theta) * sin(gamma) ;
    float x = radius * cos(theta) * cos(gamma) ;

    gamma += 2*M_PI / 100 ;
    if(gamma>=M_PI)
    {
        gamma = -M_PI ;
        theta += 2*M_PI/100 ;
    }
    ShadowLightPositions[4] = glm::vec4(x,y,z,1.0) ;

}


// 从默认缓冲中导出color buffer 用于 rough reflection rgb 联合爆搜
void saveImage()
{
    glBindFramebuffer(GL_FRAMEBUFFER,0) ;
    glReadBuffer(GL_FRONT) ;
    std::string directory = "D:\\baosou1\\" ;
    std::string path = directory+std::to_string(currentrough)
                       +"_"+std::to_string(currentreflection)
                       +"_"+std::to_string(currentrgbt.x)
                       +"_"+std::to_string(currentrgbt.y)
                       +"_"+std::to_string(currentrgbt.z)
                       +".tiff" ;
    unsigned  char* data = new unsigned char[SCR_HEIGHT*SCR_WIDTH*4];

    glPixelStorei(GL_PACK_ALIGNMENT,4) ;
    glReadPixels(0,0,SCR_WIDTH,SCR_HEIGHT,GL_RGBA,GL_UNSIGNED_BYTE,data) ;
    writeRGBATIFF(data,SCR_WIDTH,SCR_HEIGHT,path) ;
    currentname++ ;
    delete data ;
}

// 从默认缓冲中导出color buffer 用于 光源位置爆搜
void saveShadowImage()
{
    glBindFramebuffer(GL_FRAMEBUFFER,0) ;
    glReadBuffer(GL_FRONT) ;
    std::string directory = "D:\\baosou4\\" ;
    std::string path = directory+std::to_string(ShadowLightPositions[4].x)
                       +"_"+std::to_string(ShadowLightPositions[4].y)
                       +"_"+std::to_string(ShadowLightPositions[4].z)
                       +".tiff" ;
    unsigned  char* data = new unsigned char[SCR_HEIGHT*SCR_WIDTH*4];

    glPixelStorei(GL_PACK_ALIGNMENT,4) ;
    glReadPixels(0,0,SCR_WIDTH,SCR_HEIGHT,GL_RGBA,GL_UNSIGNED_BYTE,data) ;
    writeRGBATIFF(data,SCR_WIDTH,SCR_HEIGHT,path) ;
    currentname++ ;
    delete data ;
}


#endif