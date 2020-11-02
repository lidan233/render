//
// Created by 李源 on 2020-08-01.
//
#include "Model.h"
void Mesh::Draw(Shader shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    unsigned int showdowNr = 1;
    unsigned int PshowdowNr = 1;
    unsigned int ChoooseNr = 1 ;
    glBindVertexArray(VAO) ;
    for(unsigned int i = 0;i<textures.size();i++)
    {

        std::string number ;
        std::string name = textures[i].type;


        if(name == "texture_diffuse"){
            number =  std::to_string(diffuseNr++) ;
        }else if(name == "texture_specular")
        {
            number = std::to_string(specularNr++) ;
        }else if(name == "texture_height")
        {
            number = std::to_string(heightNr++) ;
        }else if(name == "texture_normal")
        {
            number = std::to_string(normalNr++) ;
        }else if(name == "texture_shadow")
        {
            number = std::to_string(showdowNr++) ;
        }else if(name == "texture_shadow_cube")
        {
            number = std::to_string(PshowdowNr++) ;
        }else if(name == "texture_choose_buffer")
        {
            number = std::to_string(ChoooseNr++) ;
        }
        //shader.setInt((name+number).c_str(),i) ;

//        glUniform1i(glGetUniformLocation(shader.ID,(name+number).c_str()),i) ;
        glActiveTexture(GL_TEXTURE0+i) ;
        if(name == "texture_shadow_cube")
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP,textures[i].id) ;
//            std::cout<<textures[i].id<<" is bind to "<<i<<std::endl ;
        }else{
            glBindTexture(GL_TEXTURE_2D,textures[i].id) ;
        }

    }



    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0) ;

    glBindVertexArray(0) ;
    glActiveTexture(GL_TEXTURE0) ;
}

void Mesh::DrawShadow(Shader shader) {
    glBindVertexArray(VAO) ;
    glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0) ;
    glBindVertexArray(0) ;
    //glActiveTexture(GL_TEXTURE0) ;
}



Mesh::Mesh(std::vector<Vertex> vertices,
           std::vector<unsigned int> indices,
           std::vector<Texture> textures,
           bool iscalculateCurvature) {
    this->vertices = vertices ;
    this->indices = indices ;
    this->textures = textures ;

    if(iscalculateCurvature)
    {
        calculateAllMinCurvature(this->vertices,this->indices) ;
    }

    setUpMesh() ;
}


void Mesh::setUpMesh() {
    glGenVertexArrays(1,&VAO) ;
    glGenBuffers(1,&VBO) ;
    glGenBuffers(1,&EBO) ;

    glBindVertexArray(VAO) ;
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(Vertex),&vertices[0],GL_STATIC_DRAW) ;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO) ;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(unsigned int),&indices[0],GL_STATIC_DRAW) ;

    glEnableVertexAttribArray(0) ;
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0) ;

    glEnableVertexAttribArray(1) ;
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,Normal)) ;

    glEnableVertexAttribArray(2) ;
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,TexCoords)) ;

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,Tangent)) ;

    glEnableVertexAttribArray(4) ;
    glVertexAttribPointer(4,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,Bitangent)) ;

    glEnableVertexAttribArray(5) ;
    glVertexAttribPointer(5,1,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,curvature)) ;

    glEnableVertexAttribArray(6) ;
    glVertexAttribPointer(6,1,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,modelId)) ;


    glBindVertexArray(0) ;

}