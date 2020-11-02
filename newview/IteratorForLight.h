//
// Created by MQ on 2020/8/31.
// 这个文件包含值迭代器和颜色迭代器

//值迭代器 开始 结束 步长

//颜色迭代器 要考虑输入颜色是rgb 但输入波动范围是hsv 输出是rgb

// 输入的颜色起点(rgb)
// 输入颜色的波动范围 (hsv) h波动范围 s波动范围 v波动范围
// 迭代器 给出下一个颜色应该是什么样子
// 如果你不进行颜色爆搜，那么你将永远不会使用该文件

#ifndef LEARNOPENGL_ITERATORFORLIGHT_H
#define LEARNOPENGL_ITERATORFORLIGHT_H
#include <algorithm>
#include <iostream>


#include "Rgb2Hsv.h"


//值迭代器 开始 结束 步长数目
template<typename T>
class Value_Iterator{
protected:
    T begin;
    T end ;
    T step ;
    int current ;

public:
    Value_Iterator(T begin1,T end1,T step1):
    begin(begin1),end(end1),step((end-begin)/step1),current(0)
    {}

    Value_Iterator& operator=(const Value_Iterator& src)
    {
      begin = src.begin ;
      end = src.end ;
      step = src.step ;
      current = src.current ;

    }
    // Dereference an iterator
    T& operator*()
    {
        if (begin+step*current>end)
        {
            throw std::logic_error("Cannot increment an end iterator.");
        }

        return begin+step*current;
    }
    Value_Iterator& operator++()
    {
        if (begin+step*current>end)
        {
            throw std::logic_error("Cannot increment an end iterator.");
        }
        current++ ;
        return *this;
    }
    // Postfix increment operator
    Value_Iterator operator++(int)
    {

        if (begin+step*current>end)
        {
            throw std::logic_error("Cannot increment an end iterator.");
        }
        current++ ;
        return *this;
    }


    bool operator==(const Value_Iterator& iter) const { return begin+step*current == iter.begin+iter.step*iter.current; }
    bool operator!=(const Value_Iterator& iter) const {  return begin+step*current != iter.begin+iter.step*iter.current; }

    void reset()
    {
        current = 0 ;
    }

    T get(int i)
    {
        return begin+step*i ;
    }
};


//颜色迭代器 要考虑输入颜色是rgb 但输入波动范围是hsv 输出是rgb
// 输入颜色rgb
// hsvbound hsv波动范围
// step 步长数目
template<typename T>
class Light_Iterator{
protected:
    double r,g,b,h,s,v ;
    double fluh,flus,fluv ;
    double hstep,sstep,vstep ;
    T begin ;

public:
     Light_Iterator(T& rgb,glm::vec3& hsvbound,int step = 1000)
    :r(rgb.r),g(rgb.g),b(rgb.b)
    {
        T hsv = rgb2hsv(rgb) ;
        h = hsv.x ;
        s = hsv.y ;
        v = hsv.z ;

        h -= 0.5*hsvbound.x ;
        s -= 0.5*hsvbound.y ;
        v -= 0.5*hsvbound.z ;

        T t = T(h,s,v) ;
        begin = t ;


        T  rgbresult= hsv2rgb(t) ;
        r = rgbresult.r ;
        g = rgbresult.g ;
        b = rgbresult.b ;

        hstep = (hsvbound.x) /double(step) ;
        sstep = (hsvbound.y) /double(step) ;
        vstep = (hsvbound.z) /double(step) ;

        fluh = h+hsvbound.x ;
        flus = s+hsvbound.y ;
        fluv = v+hsvbound.z ;

    }

    Light_Iterator& operator=(const Light_Iterator& src)
    {
       r = src.r ;
       g = src.g ;
       b = src.b ;

       h = src.h ;
       s = src.s ;
       v = src.v ;

    }
    // Dereference an iterator
    T& operator*()
    {
        if (h >= fluh || s>=flus || v>=fluv)
        {
            throw std::logic_error("Cannot increment an end iterator.");
        }

        return T(r,g,b);
    }

    T& addh()
    {
        if (h >= fluh || s>=flus || v>=fluv)
        {
            throw std::logic_error("Cannot increment an end iterator.");
        }
        h += hstep ;

        T t = T(h,s,v) ;
        T rgb = hsv2rgb<T>(t);
        r = rgb.r ;
        g = rgb.g ;
        b = rgb.b ;
        return T(r,g,b);
    }
    T& adds()
    {
        if (h >= fluh || s>=flus || v>=fluv)
        {
            throw std::logic_error("Cannot increment an end iterator.");
        }
        s += sstep ;

        T t = T(h,s,v) ;
        T rgb = hsv2rgb<T>(t);
        r = rgb.r ;
        g = rgb.g ;
        b = rgb.b ;
        return T(r,g,b);
    }
    T& addv()
    {
        if (h >= fluh || s>=flus || v>=fluv)
        {
            throw std::logic_error("Cannot increment an end iterator.");
        }
        v += vstep ;

        T t = T(h,s,v) ;
        T rgb = hsv2rgb<T>(t);
        r = rgb.r ;
        g = rgb.g ;
        b = rgb.b ;
        return T(r,g,b);
    }



    // Prefix increment operator
    Light_Iterator& operator++()
    {
        if (h >= fluh || s>=flus || v>=fluv)
        {
            throw std::logic_error("Cannot increment an end iterator.");
        }
        h += hstep ;
        s += sstep ;
        v += vstep ;

        T t = T(h,s,v) ;
        T rgb = hsv2rgb<T>(t);
        r = rgb.r ;
        g = rgb.g ;
        b = rgb.b ;

        return *this;
    }
    // Postfix increment operator
    Light_Iterator operator++(int)
    {

        if (h >= fluh || s>=flus || v>=fluv)
        {
            throw std::logic_error("Cannot increment an end iterator.");
        }
        auto temp = *this;

        h += hstep ;
        s += sstep ;
        v += vstep ;

        T t = T(h,s,v) ;
        T rgb = hsv2rgb<T>(t);
        r = rgb.r ;
        g = rgb.g ;
        b = rgb.b ;
        // Increment the value by the range step
        return temp;                                 // The iterator before it's incremented
    }
    // Comparisons

    bool operator==(const Light_Iterator& iter) const { return h == iter.h && s == iter.s && v == iter.v; }
    bool operator!=(const Light_Iterator& iter) const { return h != iter.h && s != iter.s && v != iter.v;}

    void reset(int i)
    {
        if(i&0x001!=0) r = begin.x ;
        if(i&0x010!=0) g = begin.y ;
        if(i&0x100!=0) b = begin.z ;

        T t = T(r,g,b) ;
        T hsv = rgb2hsv<T>(t) ;
        h = hsv.x ;
        s = hsv.y ;
        v = hsv.z ;

    }

    T get(int i)
    {
        double ht = begin.x ;
        double st = begin.y ;
        double vt = begin.z ;

        ht += i/25 * hstep ;
        i -= i/25*25 ;

        st += i/5* sstep ;
        i -= i/5*5 ;

        vt += i*vstep ;

        T t = T(ht,st,vt) ;
        T rgb = hsv2rgb<T>(t);
        r = rgb.r ;
        g = rgb.g ;
        b = rgb.b ;

        return rgb;
    }


};
#endif //LEARNOPENGL_ITERATORFORLIGHT_H
