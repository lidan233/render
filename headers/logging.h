//
// Created by 李源 on 2020-08-14.
//

#ifndef LEARNOPENGL_LOGGING_H
#define LEARNOPENGL_LOGGING_H

enum INFO{ERROR,WARNNING,INFO,NOTE} info ;


void log(enum INFO level,std::string str)
{
    switch(level)
    {
        case ERROR:
            std::cout<<"ERROR: "<<str<<std::endl ;
        case WARNNING:
            std::cout<<"WARNNING: "<<str<<std::endl ;

        case NOTE:
            std::cout<<"NOTE: "<<str<<std::endl ;

        case INFO:
            std::cout<<"INFO: "<<str<<std::endl ;
    }
    std::cout<<str<<std::endl;
}
#endif //LEARNOPENGL_LOGGING_H
