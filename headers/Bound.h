//
// Created by 李源 on 2020-08-12.
//

#ifndef LEARNOPENGL_BOUND_H
#define LEARNOPENGL_BOUND_H

#include <glm/glm.hpp>
#include <math.h>

class Bound{
public:
    Bound():
    minx(1000) ,
    miny(1000) ,
    minz(1000) ,
    maxx(-1000) ,
    maxy(-1000) ,
    maxz(-1000)
    {};

    Bound( Bound const& other):
            minx(other.minx) ,
            miny(other.miny) ,
            minz(other.minz) ,
            maxx(other.maxx) ,
            maxy(other.maxy) ,
            maxz(other.maxz)
    {}


    Bound(float minx,float miny,float minz,float maxx, float maxy, float maxz) :
        minx(minx) ,
        miny(miny) ,
        minz(minz) ,
        maxx(maxx) ,
        maxy(maxy) ,
        maxz(maxz)
    {}

    void addPoint(glm::vec3& vector) {
        if(vector.x<minx) minx = vector.x ;
        if(vector.x>maxx) maxx = vector.x ;
        if(vector.y<miny) miny = vector.y ;
        if(vector.y>maxy) maxy = vector.y ;
        if(vector.z<minz) minz = vector.z ;
        if(vector.z>maxz) maxz = vector.z ;
    }

    glm::vec3 getMinBound()
    {
        return glm::vec3(minx,miny,minz) ;
    }

    glm::vec3 getMaxBound()
    {
        return glm::vec3(maxx,maxy,maxz) ;
    }

    void merge(Bound& bound)
    {
        if(minx>bound.minx) minx = bound.minx ;
        if(maxx<bound.maxx) maxx = bound.maxx ;
        if(miny>bound.miny) miny = bound.miny ;
        if(maxy<bound.maxy) maxy = bound.maxy ;
        if(minz>bound.minz) minz = bound.minz ;
        if(maxz<bound.maxz) maxz = bound.maxz ;
    }

    glm::vec3 Move2Zero()
    {
        glm::vec3 mid = glm::vec3((minx+maxx)/2,(miny+maxy)/2,(minz+maxz)/2) ;
        return glm::vec3(0.0)-mid;
    }
    glm::vec3 getMiddle()
    {
        glm::vec3 mid =  glm::vec3((minx+maxx)/2,(miny+maxy)/2,(minz+maxz)/2) ;
        return mid ;
    }
    bool isUp_z(Bound bound)
    {
        if(abs(minz-bound.minz) > abs(bound.maxz -maxz ))
        {
            return true ;
        }else{
            return false ;
        }
    }

private:
    float minx,miny,minz ;
    float maxx,maxy,maxz ;
};
#endif //LEARNOPENGL_BOUND_H
