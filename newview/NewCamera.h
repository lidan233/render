//
// Created by MQ on 2020/8/24.
//

#ifndef LEARNOPENGL_NEWCAMERA_H
#define LEARNOPENGL_NEWCAMERA_H



#include <glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

#include "logging.h"

enum NCamera_Movement {
    ZERO,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


const float NYAW         = 180.0f;
const float NPITCH       =  0.0f;
//const float NYAW         = 220.0f;
//const float NPITCH       =  -57.680f;
const float NSPEED       =  2.5f;
const float NSENSITIVITY =  2.0f;
const float NZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class NCamera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 Target ;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    float radius ;

    // constructor with vectors
    NCamera(glm::vec3 front=glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3 target = glm::vec3(0.0,0.0,0.0) ,
            float yaw = NYAW,
            float pitch = NPITCH)
                :MovementSpeed(NSPEED),
                MouseSensitivity(NSENSITIVITY),
                Zoom(NZOOM)
    {
        Front = front ;
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        Target = target ;
        radius = glm::length(Position-Target) ;

        updateCameraVectors();
    }


    // constructor with scalar values
    NCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(NSPEED),
    MouseSensitivity(NSENSITIVITY),
    Zoom(NZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        Target = glm::vec3(0,0,0) ;
        radius = glm::length(Target - Position) ;
        updateCameraVectors();
    }


    void setCenter(glm::vec3 center)
    {
        glm::vec3 dir = glm::normalize(Position-Target) ;
        Target = center ;
        Position = Target + radius*dir ;
        Front = glm::normalize(Target-Position) ;
    }
    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    glm::mat4 UpGetViewMatrix()
    {
        return glm::lookAt(Position+(-Up+glm::vec3(10)*Right), Position + Front, Up);
    }

    glm::vec3 UpGetPosition()
    {
        return Position+(-Up+glm::vec3(10)*Right) ;
    }


    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(NCamera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;

        radius = glm::length(Target - Position) ;
//        log(INFO," " +std::to_string(Position[0]) +" " +std::to_string(Position[1])+" " +std::to_string(Position[2]));
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   -= yoffset;
        Pitch += xoffset;

        updateCameraVectors();
    }


    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:

    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
//         also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
         Position = Target - radius * Front ;


//        log(INFO,"PITCH: "+std::to_string(Pitch)+" Yaw: "+std::to_string(Yaw)) ;
//        log(INFO,"up :" +std::to_string(Up[0]) +" " +std::to_string(Up[1])+" " +std::to_string(Up[2]));
//        log(INFO,"Front:" +std::to_string(Front[0]) +" " +std::to_string(Front[1])+" " +std::to_string(Front[2]));
//        log(INFO,"Position " +std::to_string(Position[0])+" " +std::to_string(Position[1]) +" "+" "+std::to_string(Position[2])) ;
//        log(INFO,"ViewMatrix"+glm::to_string(this->GetViewMatrix())+"\n") ;
    }
};




#endif //LEARNOPENGL_NEWCAMERA_H
