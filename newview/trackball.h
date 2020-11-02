//
// Created by MQ on 2020/8/24.
//

#ifndef LEARNOPENGL_TRACKBALL_H
#define LEARNOPENGL_TRACKBALL_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>

class trackball {
private:
    glm::ivec2 _center_position ;
    glm::ivec2 _drag_start_position ;
    float _radius ;
    bool _dragged ;


    glm::vec3 map_to_sphere(const glm::ivec2 &point) ;

public:
    trackball(float radius) :_radius(radius) {}

    void drag_start(int x,int y) ;
    void drag_update(int x,int y) ;
    void drag_end() ;
    bool dragged() const {return _dragged ;}


    glm::quat& rotate(glm::quat &orientation,int x,int y) ;

    glm::vec2 direction(int x,int y) ;

    void center(float x,float y)
    {
        _center_position.x = x;
        _center_position.y = y ;
    }
};


#endif //LEARNOPENGL_TRACKBALL_H
