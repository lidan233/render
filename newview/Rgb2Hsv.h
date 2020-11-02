//
// Created by MQ on 2020/8/31.
// 工具函数，rgb和hsv之间互相转换
//

#ifndef LEARNOPENGL_RGB2HSV_H
#define LEARNOPENGL_RGB2HSV_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

template<typename T>
T rgb2hsv(T& rgbin)
{
    T        hsvout;
    double      min, max, delta;

    min = rgbin.r < rgbin.g ? rgbin.r : rgbin.g;
    min = min  < rgbin.b ? min  : rgbin.b;

    max = rgbin.r > rgbin.g ? rgbin.r : rgbin.g;
    max = max  > rgbin.b ? max  : rgbin.b;

    hsvout.z = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        hsvout.y = 0;
        hsvout.x = 0; // undefined, maybe nan?
        return hsvout;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        hsvout.y = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0
        // s = 0, h is undefined
        hsvout.y = 0.0;
        hsvout.x = NAN;                            // its now undefined
        return hsvout;
    }
    if( rgbin.r >= max )                           // > is bogus, just keeps compilor happy
        hsvout.x = ( rgbin.g - rgbin.b ) / delta;        // between yellow & magenta
    else
    if( rgbin.g >= max )
        hsvout.x = 2.0 + ( rgbin.b - rgbin.r ) / delta;  // between cyan & yellow
    else
        hsvout.x = 4.0 + ( rgbin.r - rgbin.g ) / delta;  // between magenta & cyan

    hsvout.x *= 60.0;                              // degrees

    if( hsvout.x < 0.0 )
        hsvout.x += 360.0;

    return hsvout;
}

template <typename T>
T hsv2rgb(T& hsvin)
{
    double      hh, p, q, t, ff;
    long        i;
    T   rgbout;

    if(hsvin.y <= 0.0) {       // < is bogus, just shuts up warnings
        rgbout.r = hsvin.z;
        rgbout.g = hsvin.z;
        rgbout.b = hsvin.z;
        return rgbout;
    }
    hh = hsvin.x;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = hsvin.z * (1.0 - hsvin.y);
    q = hsvin.z * (1.0 - (hsvin.y * ff));
    t = hsvin.z * (1.0 - (hsvin.y * (1.0 - ff)));

    switch(i) {
        case 0:
            rgbout.r = hsvin.z;
            rgbout.g = t;
            rgbout.b = p;
            break;
        case 1:
            rgbout.r = q;
            rgbout.g = hsvin.z;
            rgbout.b = p;
            break;
        case 2:
            rgbout.r = p;
            rgbout.g = hsvin.z;
            rgbout.b = t;
            break;

        case 3:
            rgbout.r = p;
            rgbout.g = q;
            rgbout.b = hsvin.z;
            break;
        case 4:
            rgbout.r = t;
            rgbout.g = p;
            rgbout.b = hsvin.z;
            break;
        case 5:
        default:
            rgbout.r = hsvin.y;
            rgbout.g = p;
            rgbout.b = q;
            break;
    }
    return rgbout;
}


#endif //LEARNOPENGL_RGB2HSV_H
