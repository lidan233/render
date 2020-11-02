//
// Created by MQ on 2020/8/21.
//

#ifndef LEARNOPENGL_NEWVIEW_H
#define LEARNOPENGL_NEWVIEW_H


#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

#include <stdio.h>
#include <functional>
#include <sys/types.h>
#include <sstream>

#include <glm/gtx/string_cast.hpp>

#include "dirent.h"
#include "CheckShader.h"
#include "Model.h"
#include "NewCamera.h"
#include "stb_image.h"
#include "WriteImage.h"

glm::vec3 cameraPos   = glm::vec3(70.0f, 0.0f,  0.0f);
glm::vec3 cameraFront = glm::vec3(-1.0f, 0.0f, 0.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 0.0f,  1.0f);
glm::vec3 camertarget = glm::vec3(0.0f,0.0f,0.0f) ;


NCamera camera(cameraFront,cameraPos,cameraUp,camertarget) ;

bool gammaEnabled = false;
bool gammaKeyPressed = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = 400, lastY = 300;

float yaw = 0 ;
float pitch = 0 ;
bool firstMouse = true ;


float fov = 45.0f;

bool rotate = true ;

bool blin = false ;
GLboolean shadows = true;
// Global variables
GLuint woodTexture;
GLuint planeVAO;

float mylightbili = 0.8 ;
float texturebili = 0.2 ;



const unsigned int SHADOW_WIDTH = 1024*3, SHADOW_HEIGHT = 1024*3;
const unsigned int SCR_WIDTH = 1280, SCR_HEIGHT = 720;

#define LIGHT_SIZE 5
#define SHADOW_SIZE 3

unsigned int depthMapFBO[LIGHT_SIZE];
unsigned int depthMap[LIGHT_SIZE];

unsigned int chooseMapFBO ;
unsigned int chooseMap ;




float cursorPosx = -1 ;
float cursorPosy = -1 ;
bool choose = false ;


glm::vec4 lightPositions[] = {

        glm::vec4(-150.0, 100.0, 50.0, 1.0) ,
        glm::vec4(150.0, -100.0, 50.0, 1.0),
        glm::vec4(0.0, 200.0, 100.0, 1.0),
//        glm::vec4(0.0,0,150,1.0),
//        glm::vec4(0.0,120,-100,1.0),
//        glm::vec4(0.0,-120,-100,1.0),
        glm::vec4(0.0,20,-100,1.0),
//        glm::vec4(0.0,-20,-100,1.0),
        glm::vec4(0.0,-20,-100,1.0),

};

glm::vec4 ShadowLightPositions[] = {
        glm::vec4(0.0,60,148,1.0),
        glm::vec4(0,100,100,1.0),
        glm::vec4(100,0,100,1.0),
        glm::vec4(100,100,0,1.0),
        glm::vec4(0.0,20,-100,1.0),
//        glm::vec4(0.0,-20,-100,1.0),


};




float near_plane = 1.0f;
float far_plane  = 300.0f;

bool useUp = false ;
//


void showFPS(GLFWwindow * window,int winw, int winh) {

    static double t0 = 0.0;
    static int frames = 0;
    static char titlestr[200];
    double t, fps;

    // Get current time
    t = glfwGetTime();  // Number of seconds since glfwInit()
    // If one second has passed, or if this is the very first frame
    if( (t - t0) > 1.0 || frames == 0 )
    {
        fps = (double)frames / (t - t0);
        sprintf(titlestr, "%dx%d pixels, %.1f FPS -> %.1f Mpixels/s",
                winw, winh, fps, winw*winh*fps*1e-6);
        glfwSetWindowTitle(window,titlestr);
        t0 = t;
        frames = 0;
    }
    frames ++;
}


void saveChooseMaptoImage()
{
    unsigned char * front= (unsigned char*) malloc(4 * SCR_WIDTH * SCR_HEIGHT*sizeof(unsigned char ));
    glBindTexture(GL_TEXTURE_2D,chooseMap);

    glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_DEPTH_COMPONENT,GL_FLOAT,front);
    writeRGBATIFF(front,SCR_WIDTH,SCR_HEIGHT,"C:\\Users\\MQ\\Desktop\\choosebuffer.tiff") ;
}

//輸出cubemap到
void SaveCubeMapToImage(int i)	// this save all 6 faces of cube map into images in harddrive
{
    int cubemaptex = depthMap[i] ;
    float* front= (float*) malloc(1 * 1024 * 1024*sizeof(float));


    glBindTexture(GL_TEXTURE_CUBE_MAP,cubemaptex) ;


    glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,GL_DEPTH_COMPONENT,GL_FLOAT,front);
    writeTiff(front,SHADOW_HEIGHT,SHADOW_WIDTH,"C:\\Users\\MQ\\Desktop\\"+std::to_string(i)+"posX.tiff") ;



    glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0,GL_DEPTH_COMPONENT,GL_FLOAT,front);
    writeTiff(front,SHADOW_HEIGHT,SHADOW_WIDTH,"C:\\Users\\MQ\\Desktop\\"+std::to_string(i)+"negX.tiff") ;



    glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0,GL_DEPTH_COMPONENT,GL_FLOAT,front);
    writeTiff(front,SHADOW_HEIGHT,SHADOW_WIDTH,"C:\\Users\\MQ\\Desktop\\"+std::to_string(i)+"posY.tiff") ;


    glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0,GL_DEPTH_COMPONENT,GL_FLOAT,front);
    writeTiff(front,SHADOW_HEIGHT,SHADOW_WIDTH,"C:\\Users\\MQ\\Desktop\\"+std::to_string(i)+"negY.tiff") ;



    glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0,GL_DEPTH_COMPONENT,GL_FLOAT,front);
    writeTiff(front,SHADOW_HEIGHT,SHADOW_WIDTH,"C:\\Users\\MQ\\Desktop\\"+std::to_string(i)+"posZ.tiff") ;


    glGetTexImage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0,GL_DEPTH_COMPONENT,GL_FLOAT,front);
    writeTiff(front,SHADOW_HEIGHT,SHADOW_WIDTH,"C:\\Users\\MQ\\Desktop\\"+std::to_string(i)+"negZ.tiff") ;

//    giglDeleteTextures(1,&CarrierText);


    GLenum err;
//    while ((err = glGetError()) != GL_NO_ERROR) {
////        std::cerr << "OpenGL error: " << err << std::endl;
//    }
    // write this images down


}



unsigned int loadTexture(const char *path)
{
    unsigned int textureId ;
    glGenTextures(1,&textureId) ;

    int width,height,nrComponents;
    unsigned char *data = stbi_load(path,&width,&height,&nrComponents,0) ;

    if(data)
    {
        GLenum format ;
        if(nrComponents == 1)
        {
            format = GL_RED ;
        }else if(nrComponents == 3)
        {
            format = GL_RGB ;
        }else if(nrComponents == 4)
        {
            format = GL_RGBA ;
        }

        glBindTexture(GL_TEXTURE_2D,textureId) ;
        glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,data) ;
        glGenerateMipmap(GL_TEXTURE_2D) ;

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT) ;
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR) ;
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR) ;

        stbi_image_free(data) ;
    }else{
        std::cout<<"Texture failed to load at path:"<<path<<std::endl ;
        stbi_image_free(data) ;
    }
    return textureId ;
}


void framebuffer_size_callback(GLFWwindow *window,int height,int width)
{
    glViewport(0,0,width,height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    cursorPosx = xpos/(float)SCR_WIDTH ;
    cursorPosy = ypos/(float)SCR_HEIGHT ;


    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.01f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw   += xoffset;
    pitch += yoffset;




    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = -cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    cameraFront = glm::normalize(front);

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if(state == GLFW_PRESS){
        camera.ProcessMouseMovement(xoffset,yoffset) ;
    }else{

    }

//    std::cout<<std::to_string(xpos)<<" "<<std::to_string(ypos)<<std::endl ;
}


inline void process_Input(GLFWwindow *window,NCamera* camera)
{
//    extern float lastFrame ;
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float cameraSpeed = 2.5f * deltaTime;
    NCamera_Movement add  = ZERO;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        add = FORWARD ;
        std::cout<<"come on"<<std::endl ;
    }

//        cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        add = BACKWARD ;
//        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        add = LEFT ;
//        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        add = RIGHT ;

    if(glfwGetKey(window,GLFW_KEY_B) == GLFW_PRESS)
    {
        blin = true ;
    }
    if(glfwGetKey(window,GLFW_KEY_P) == GLFW_PRESS)

    {
        blin = false;
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !gammaKeyPressed)
    {
        gammaEnabled = !gammaEnabled;
        gammaKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE)
    {
        gammaKeyPressed = false;
    }


//        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if(glfwGetKey(window,GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window,true) ;
    }


    if(  glfwGetKey(window,GLFW_KEY_M) == GLFW_PRESS)
    {
        if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            mylightbili -= 0.001 ;
        } else{
            mylightbili += 0.001 ;
        }
        std::cout<<"mylight: "<<mylightbili <<" otherlight"<<texturebili<<std::endl ;
    }

    if(  glfwGetKey(window,GLFW_KEY_O) == GLFW_PRESS)
    {
        if(glfwGetKey(window,GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            texturebili -= 0.001 ;
        }else{
            texturebili += 0.001 ;
        }

        std::cout<<"mylight: "<<mylightbili <<" otherlight"<<texturebili<<std::endl ;
    }




    if(glfwGetKey(window,GLFW_KEY_L) == GLFW_PRESS)
    {
        useUp = !useUp ;
    }
    if(glfwGetKey(window,GLFW_KEY_I) == GLFW_PRESS)
    {
        for(int i = 0;i<LIGHT_SIZE;i++)
        {
            SaveCubeMapToImage(i);
        }

    }
    if(glfwGetKey(window,GLFW_KEY_U) == GLFW_PRESS)
    {

        saveChooseMaptoImage();

    }



    camera->ProcessKeyboard(add,cameraSpeed) ;
}


void genShadowBuffer()
{

    for(int t = 0; t<SHADOW_SIZE;t++)
    {
        glGenFramebuffers(1, &depthMapFBO[t]);
        glGenTextures(1, &depthMap[t]);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthMap[t]);
        for (unsigned int i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

        float boardColor[] = {1.0,1.0,1.0,1.0};
        glTexParameterfv(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_BORDER_COLOR,boardColor) ;

        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[t]);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap[t], 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}


void genChooseBuffer()
{
    glGenFramebuffers(1,&chooseMapFBO) ;
    glGenTextures(1,&chooseMap) ;
    glBindFramebuffer(GL_FRAMEBUFFER,chooseMapFBO) ;

    glBindTexture(GL_TEXTURE_2D,chooseMap) ;
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,SCR_WIDTH,SCR_HEIGHT,0,GL_RGB,GL_FLOAT,NULL) ;
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR) ;
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR) ;
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE) ;
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE) ;

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,chooseMap,0);

    unsigned int tmp ;
    glGenRenderbuffers(1,&tmp) ;
    glBindRenderbuffer(GL_RENDERBUFFER,tmp) ;
    glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,SCR_WIDTH,SCR_HEIGHT) ;
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,tmp) ;

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE) {
        std::cout<<"no complete for choose framebuffer" ;
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0) ;

}








void renderTeeth(CShader& shader,Model& model,bool isup)
{
    shader.setVec3("boundmin",model.bound.getMinBound()) ;
    shader.setVec3("boundmax",model.bound.getMaxBound()) ;
    shader.setBool("isUp",isup) ;
    shader.setInt("useUp",useUp) ;
    shader.setFloat("far_plane",far_plane) ;


    model.Draw(shader) ;

}

void renderShadowTeeth(CShader& shader,Model& model)
{

    model.DrawShadow(shader) ;
}



void renderTooth(CShader& shader ,std::vector<Model>& models,Bound& bound)
{


    for(auto it = models.begin();it!=models.end();it++)
    {
        bool t = (*it).bound.isUp_z(bound) ;
        renderTeeth(shader,*it,t) ;

    }
}
void renderLight(CShader& shader ,std::vector<Model>& models)
{
    for(auto it = models.begin();it!=models.end();it++)
    {
        (*it).Draw(shader) ;
    }
}

void renderShadowTooth(CShader& shader ,std::vector<Model>& models,Bound& bound)
{
    for(auto it = models.begin();it!=models.end();it++)
    {
        bool t = (*it).bound.isUp_z(bound) ;
        shader.setBool("isUp",t) ;
        shader.setBool("useUp",useUp) ;
        renderShadowTeeth(shader,*it) ;
    }
}


void useChooseBuffer(CShader &shader,std::vector<Model>& models,
                     glm::mat4& model,glm::mat4& view,glm::mat4& projection
                     ,Bound& bound)
{
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, chooseMapFBO);
    glClearColor(0,0,0,1) ;
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST) ;

    shader.use() ;
    shader.setMat4("model",model) ;
    shader.setMat4("view",view) ;
    shader.setMat4("projection",projection) ;

    for(auto it = models.begin();it!=models.end();it++)
    {
        bool t = (*it).bound.isUp_z(bound) ;
        shader.setBool("isUp",t) ;
        shader.setBool("useUp",useUp) ;
        (*it).DrawChooseBuffer(shader) ;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void useShadowBuffer(CShader &shader, std::vector<Model>& models,glm::mat4& Model,glm::mat4& View,Bound& bound)
{

    for(int t= 0;t<SHADOW_SIZE;t++)
    {
        glm::vec3 lightPositionWorldSpace = glm::vec3(ShadowLightPositions[t]) ;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositionWorldSpace, lightPositionWorldSpace + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositionWorldSpace, lightPositionWorldSpace + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositionWorldSpace, lightPositionWorldSpace + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositionWorldSpace, lightPositionWorldSpace + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositionWorldSpace, lightPositionWorldSpace + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPositionWorldSpace, lightPositionWorldSpace + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO[t]);
        glClear(GL_DEPTH_BUFFER_BIT);
        shader.use();
        for (unsigned int i = 0; i < 6; ++i)
            shader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        shader.setFloat("far_plane", far_plane);
        shader.setVec3("lightPos", lightPositionWorldSpace);
        shader.setMat4("model",Model) ;
        shader.setMat4("view",View) ;

        renderShadowTooth(shader,models,bound);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}


void setShadowBuffer(std::vector<Model>& models)
{

    for(auto model = models.begin();model != models.end();model++)
    {
        for(int t = 0; t< SHADOW_SIZE;t++)
        {
            (*model).setUpShadowBufferTexture(depthMap[t]) ;
        }

    }

}

void setChooseBuffer(std::vector<Model>& models)
{

    for(auto model = models.begin();model != models.end();model++)
    {
        (*model).setUpChooseBufferTexture(chooseMap) ;
    }

}


// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

//
std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}


//列出所有obj文件 放入strs
static void list_dir(const char *path,std::vector<std::string>& strs )
{
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return;
    }
    while ((entry = readdir(dir)) != NULL) {

        std::string s = entry->d_name;
        if(split(trim_copy(s),'.').size()>2 && split(trim_copy(s),'.')[2] == "obj")
        {
            printf("%s\n",entry->d_name);
            strs.push_back(s);
        }
    }
    closedir(dir);
}



//計算包圍盒 分配model ID
Bound* addModels(std::vector<Model>& models ,bool calculateCurvature,bool addLightSphere=true)
{
//    std::string path = "C:\\Users\\MQ\\Desktop\\normaldata\\";
    std::string path = "..\\normaldata\\";
    std::vector<std::string> datap ;
    Bound* res = new Bound() ;
    list_dir(path.c_str(),datap) ;
    unsigned int id = 1 ; //牙齿的model ID 从1开始计数
    for(auto it = datap.begin() ;it!=datap.end() ;it++)
    {
        std::string t =  path + *it ;
        Model model(t,calculateCurvature,id) ;
        models.push_back(model) ;
        res->merge(model.bound) ;
        id++ ;
    }
    return res ;
}


Bound* addModelsYayin(std::vector<Model>& models ,bool calculateCurvature,bool addLightSphere=true)
{
//    std::string path = "C:\\Users\\MQ\\Desktop\\normaldata\\yayin\\";
    std::string path = "..\\normaldata\\yayin\\";
    std::vector<std::string> datap ;
    Bound* res = new Bound() ;
    list_dir(path.c_str(),datap) ;
    unsigned int id = 50 ;//牙龈的model ID 从50开始计数
    for(auto it = datap.begin() ;it!=datap.end() ;it++)
    {
        std::string t =  path + *it ;
        Model model(t,calculateCurvature,id) ;
        models.push_back(model) ;
        res->merge(model.bound) ;
        id++ ;
    }
    return res ;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if(fov <= 1.0f)
        fov = 1.0f;
    if(fov >= 45.0f)
        fov = 45.0f;
}


GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)


#endif //LEARNOPENGL_NEWVIEW_H
