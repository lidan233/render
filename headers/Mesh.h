//
// Created by 李源 on 2020-07-20.
//

#ifndef LEARNOPENGL_MESH_H

#include <iostream>
#include <glad.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



#include "Shader.h"
#include "calcuateCurvature.h"




class Mesh{
public:
    std::vector<Vertex> vertices ;
    std::vector<unsigned int> indices ;
    std::vector<Texture> textures ;

    unsigned int alltexture;
    bool alltex = false ;

    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::vector<Texture> textures,
         bool iscalculateCurvature=false);


    void  Draw(Shader shader) ;
    void  DrawShadow(Shader shader) ;
    unsigned int VAO,VBO,EBO ;

    void setUpDiffuseTexture(unsigned int ID) {
        Texture texture ;
        texture.id = ID ;
        texture.type = "texture_diffuse" ;
        textures.push_back(texture) ;

    }


    void setUpShadowBufferTexture(unsigned int& ID) {
        Texture* texture = new Texture ;
        texture->id = ID ;
        texture->type = "texture_shadow_cube" ;
        textures.push_back(*texture) ;
    }

    void setUpChooseBufferTexture(unsigned int& ID) {
        Texture* texture = new Texture ;
        texture->id = ID ;
        texture->type = "texture_choose_buffer" ;
        textures.push_back(*texture) ;
    }
    void setUpParallelShadowBufferTexture(unsigned int& ID)
    {
        Texture* texture = new Texture ;
        texture->id = ID;
        texture->type = "texture_shadow" ;
        textures.push_back(*texture) ;
    }

private:


    void setUpMesh() ;


};

#endif //LEARNOPENGL_MESH_H
