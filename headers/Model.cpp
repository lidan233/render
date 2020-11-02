//
// Created by 李源 on 2020-07-30.
//

#include "Model.h"
#include "Bound.h"

void Model::processNode(aiNode *node, const aiScene *scene) {
    for(unsigned int i = 0 ;i  < node->mNumMeshes;i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]] ;
        meshs.push_back(processMesh(mesh,scene)) ;

    }

    for(unsigned int i = 0; i< node->mNumChildren ;i++)
    {
        processNode(node->mChildren[i],scene) ;
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices ;
    std::vector<unsigned int > indices ;
    std::vector<Texture> textures ;
    Bound bound ;



    for(unsigned int i= 0;i< mesh->mNumVertices ;i++)
    {
        Vertex vertex ;

        glm::vec3 vector ;
        vector.x = mesh->mVertices[i].x ;
        vector.y = mesh->mVertices[i].y ;
        vector.z = mesh->mVertices[i].z ;
        vertex.Position = vector ;
        bound.addPoint(vector) ;




        glm::vec3 vectorNormal ;
        vectorNormal.x = mesh->mNormals[i].x ;
        vectorNormal.y = mesh->mNormals[i].y ;
        vectorNormal.z = mesh->mNormals[i].z ;
        vertex.Normal = vectorNormal ;


        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vec ;
            vec.x = mesh->mTextureCoords[0][i].x ;
            vec.y = mesh->mTextureCoords[0][i].y ;
            vertex.TexCoords = vec ;
        }else{
            vertex.TexCoords = glm::vec2(0.0f,0.0f) ;
        }

        if(mesh->mTangents!=NULL)
        {
            vector.x = mesh->mTangents[i].x ;
            vector.y = mesh->mTangents[i].y ;
            vector.z = mesh->mTangents[i].z ;

            vertex.Tangent = vector ;
        }

        if( mesh->mBitangents!=NULL)
        {
            vector.x = mesh->mBitangents[i].x ;
            vector.y = mesh->mBitangents[i].y ;
            vector.z = mesh->mBitangents[i].z ;

            vertex.Bitangent = vector ;
        }
        vertex.curvature = 0.0f ;
        vertex.modelId = modelId ;
        vertices.push_back(vertex) ;

    }

    for(unsigned int i  =0 ; i< mesh->mNumFaces ;i++)
    {
        aiFace face = mesh->mFaces[i] ;
        for(unsigned int j = 0 ;j < face.mNumIndices ;j++)
        {
            indices.push_back(face.mIndices[j]) ;
        }
    }


    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex] ;

    std::vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE,"texture_diffuse") ;
    textures.insert(textures.end(),diffuseMaps.begin(),diffuseMaps.end()) ;

    std::vector<Texture> specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR,"texture_specular") ;
    textures.insert(textures.end(),specularMaps.begin(),specularMaps.end()) ;


    std::vector<Texture> normalMaps = loadMaterialTextures(material,aiTextureType_HEIGHT,"texture_normal");
    textures.insert(textures.end(),normalMaps.begin(),normalMaps.end()) ;

    std::vector<Texture> heightMaps = loadMaterialTextures(material,aiTextureType_AMBIENT,"texture_height") ;
    textures.insert(textures.end(),heightMaps.begin(),heightMaps.end()) ;


    this->bound.merge(bound) ;
    return Mesh(vertices,indices,textures,isCalculateCarvature) ;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures ;
    for(unsigned int i = 0 ;i < mat->GetTextureCount(type);i++)
    {
        aiString str;
        mat->GetTexture(type,i,&str) ;

        bool skip = false ;
        for(unsigned int j = 0 ;j< textures_loaded.size() ;j++)
        {
            if(strcmp(textures_loaded[j].path.data(),str.C_Str())==0)
            {
                textures.push_back(textures_loaded[j]) ;
                skip = true ;
                break ;

            }
        }
        if(!skip)
        {
            Texture texture ;
            texture.id = TextureFromFile(str.C_Str(),this->directory) ;
            texture.type = typeName ;
            texture.path = str.C_Str() ;
            textures.push_back(texture) ;
            textures_loaded.push_back(texture) ;
        }
    }

    return textures ;
}

void Model::loadModel(std::string path) {
    Assimp::Importer importer ;
    // 需要进一步的进行三角花+uv反转
    const aiScene * scene = importer.ReadFile(path,aiProcess_FlipUVs|aiProcess_JoinIdenticalVertices|aiProcess_Triangulate) ;
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout<<"ERROR::ASSIMP::"<<importer.GetErrorString()<<std::endl ;
        return ;
    }

    directory = path.substr(0,path.find_last_of('/'))  ;
    processNode(scene->mRootNode,scene) ;
}

void Model::setUpIsCalculateCarvature(bool wantTo)
{
    this->isCalculateCarvature = wantTo ;
}