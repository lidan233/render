//
// Created by MQ on 2020/9/9.
// 本代码包含一些基础的数据结构 如vertex texture 等
//

#ifndef NEW_BASESTRUCTURE_H
#define NEW_BASESTRUCTURE_H

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#define CLAMP_MACRO(v)    ((v)<(-1) ? (-1) : (v) > (1) ? (1) : (v))


struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal ;
    glm::vec2 TexCoords ;
    glm::vec3 Tangent ;
    glm::vec3 Bitangent ;
    float curvature  ;
    float modelId ;
};


struct Texture {
    unsigned int id ;
    std::string type ;
    std::string path ;
};


#endif //NEW_BASESTRUCTURE_H
