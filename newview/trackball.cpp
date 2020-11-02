//
// Created by MQ on 2020/8/24.
//

#include "trackball.h"


void trackball::drag_update(int x, int y) {
    if(_dragged)
    {
        _drag_start_position.x = x;
        _drag_start_position.y = y ;
    }
}


void trackball::drag_end() {
    if(_dragged)
    {
        drag_update(0.0f,0.0f) ;
        _dragged = false ;
    }
}

glm::vec2 trackball::direction(int x, int y) {
    glm::ivec2 drag_end_position(x-_center_position.x,y-_center_position.y) ;
    glm::vec2 v(drag_end_position.x - _drag_start_position.x,drag_end_position.y-_drag_start_position.y) ;
    v.y = -v.y ;
    return glm::normalize(v) ;

}

