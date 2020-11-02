//
// Created by 李源 on 2020-07-30.
//

#ifndef LEARNOPENGL_MODEL_H
#define LEARNOPENGL_MODEL_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"


#include "Mesh.h"
#include "Shader.h"

#include "Bound.h"


inline unsigned int TextureFromFile(const char *path,const std::string &directory,bool gamma = false) ;


inline unsigned int TextureFromFile(const char *path,const std::string &directory,bool gamma )
{
    std::string filename = std::string(path) ;
    filename = directory + '/' + filename ;


    unsigned int textureId ;
    glGenTextures(1,&textureId) ;

    int width,height,nrComponents;
    unsigned char *data = stbi_load(filename.c_str(),&width,&height,&nrComponents,0) ;

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

class Model {
public:


    std::vector<Texture> textures_loaded ;
    bool gammaCorrection ;
    std::vector<Mesh> meshs;
    Bound bound ;
    int modelId  = 0 ;



    Model( std::string const& path,bool needtocalCurvature=false,unsigned int id=0)
    {
        isCalculateCarvature = needtocalCurvature ;
        modelId = id ;
        loadModel(path) ;
    }

    Model(std::vector<glm::vec3> positionsin,
        std::vector<glm::vec2> uvin,
        std::vector<glm::vec3> normalsin,
        std::vector<unsigned int> indicesin ,
        unsigned int id = 0)
    {
        modelId = id ;

        std::vector<Vertex> vertices ;
        std::vector<Texture> textures ;

        for(int i = 0;i<positionsin.size();i++)
        {
            Vertex v ;
            v.Position = positionsin[i] ;
            v.Normal = normalsin[i] ;
            v.TexCoords = uvin[i] ;
            v.modelId = modelId ;
            vertices.push_back(v) ;
        }

        Mesh me = Mesh(vertices,indicesin,textures,false) ;
        meshs.push_back(me) ;
    }


    Model(Model const& model)
    : textures_loaded(model.textures_loaded),
    gammaCorrection(model.gammaCorrection),
    meshs(model.meshs) ,
    bound(model.bound) ,
    isCalculateCarvature(model.isCalculateCarvature),
    modelId(model.modelId)
    {

    }

    void Draw(Shader shader)
    {
        for(unsigned int i = 0 ;i <meshs.size();i++)
        {
            meshs[i].Draw(shader) ;
        }
    }

    void DrawChooseBuffer(Shader shader)
    {
        for(unsigned int i = 0 ;i <meshs.size();i++)
        {
            meshs[i].Draw(shader) ;
        }
    }

    void DrawShadow(Shader shader)
    {
        for(unsigned int i = 0 ;i <meshs.size();i++)
        {
            meshs[i].DrawShadow(shader) ;
        }
    }

    void setUpDiffuseTexture(unsigned int Texture)
    {
        for(unsigned int i = 0 ;i <meshs.size();i++)
        {
            meshs[i].setUpDiffuseTexture(Texture) ;
        }
    }

    void setUpShadowBufferTexture(unsigned int& Texture)
    {
        for(unsigned int i = 0 ;i <meshs.size();i++)
        {
            meshs[i].setUpShadowBufferTexture(Texture) ;
        }
    }
    void setUpChooseBufferTexture(unsigned int& Texture)
    {
        for(unsigned int i = 0 ;i < meshs.size();i++)
        {
            meshs[i].setUpChooseBufferTexture(Texture) ;
        }
    }

    void setUpParallelShadowBufferTexture(unsigned int& Texture)
    {
        for(unsigned int i = 0 ;i < meshs.size();i++)
        {
            meshs[i].setUpParallelShadowBufferTexture(Texture) ;
        }
    }


    void setUpIsCalculateCarvature(bool wantTo) ;

private:

    std::string directory ;
    bool isCalculateCarvature = false ;
    void loadModel(std::string path) ;
    void processNode(aiNode *node,const aiScene* scene) ;
    Mesh processMesh(aiMesh *mesh ,const aiScene* scene ) ;
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat,aiTextureType type,std::string typeName) ;



};



#endif //LEARNOPENGL_MODEL_H
