//
// Created by 李源 on 2020-08-18.
// NOTE：这个文件是用来计算曲率的，
// 该文件包含了4种曲率的计算方法，平均曲率，高斯曲率，最大曲率，最小曲率
// 输入节点和indices 输出每个节点的曲率
// 已知：平均曲率体现的是曲面的曲率的变化
// 高斯曲率体现的是曲面从平滑到凹凸不平的那个部分
// 最大曲率体现的是曲面凸出的部分
// 最小曲率体现的是曲面凹陷的部分
// 节点的最大曲率和最小曲率是没有办法直接计算的 因为不知道方向 但是可以根据平均曲率和高斯曲率合作计算出
// 本代码考虑到只是牙齿凹陷的部分需要关注 所以我们选择最小曲率


#ifndef LEARNOPENGL_CALCUATECURVATURE_H
#define LEARNOPENGL_CALCUATECURVATURE_H


#include <algorithm>
#include "basestructure.h"

inline glm::dvec3 ComputeNormalDirection(glm::dvec3 v1, glm::dvec3 v2,
                                   glm::dvec3 v3)
{
    glm::dvec3 n ;

    double ax, ay, az, bx, by, bz;

    ax = v3[0] - v2[0]; ay = v3[1] - v2[1]; az = v3[2] - v2[2];
    bx = v1[0] - v2[0]; by = v1[1] - v2[1]; bz = v1[2] - v2[2];

    n[0] = (ay * bz - az * by);
    n[1] = (az * bx - ax * bz);
    n[2] = (ax * by - ay * bx);

    return n ;
}






inline unsigned int  getNeighborTriangleIndex(unsigned int now,int from,int end,std::vector<std::vector<unsigned int>>& point2Triangle)
{
    std::vector<unsigned int> first = point2Triangle[from] ;
    std::vector<unsigned int> second = point2Triangle[end] ;

    sort(first.begin(), first.end());
    sort(second.begin(),second.end()) ;

    auto first_cur = first.begin() ;
    auto second_cur = second.begin() ;

    while(first_cur!=first.end() && second_cur!=second.end())
    {
        if((*first_cur)<(*second_cur))
        {
            first_cur++ ;
        }else if((*first_cur)>(*second_cur))
        {
            second_cur++ ;
        }else{
            if((*first_cur)!=now)
            {

                return *first_cur ;
            }
            first_cur++ ;
            second_cur++ ;
        }
    }

    return -1 ;
}

inline glm::dvec3 calculateNormal(std::vector<Vertex>& vertices,std::vector<unsigned int >& indices,unsigned int index)
{
    glm::dvec3 first = vertices[indices[index*3]].Position ;
    glm::dvec3 second = vertices[indices[index*3+1]].Position ;
    glm::dvec3 third  = vertices[indices[index*3+2]].Position ;

    glm::dvec3 n = ComputeNormalDirection(first,second,third) ;
    double length = 0.0;
    if ( (length = sqrt((n[0]*n[0] + n[1]*n[1] + n[2]*n[2]))) != 0.0 )
    {
        n[0] /= length;
        n[1] /= length;
        n[2] /= length;
    }

    return n ;
}

inline double calculateArea(std::vector<Vertex>& vertices,std::vector<unsigned int >& indices,unsigned int index)
{
    glm::dvec3 first = vertices[indices[index*3]].Position ;
    glm::dvec3 second = vertices[indices[index*3+1]].Position ;
    glm::dvec3 third  = vertices[indices[index*3+2]].Position ;

    glm::dvec3 n = ComputeNormalDirection(first,second,third) ;

    return 0.5*glm::length(n);
}




inline double calculateAngleCur(std::vector<Vertex>& vertices,std::vector<unsigned int >& indices,unsigned int base,unsigned int neigh,glm::dvec3 intersectEdge)
{

    double result = 0.0 ;
    glm::dvec3 baseNormal = calculateNormal(vertices,indices,base) ;
    glm::dvec3 neighNormal = calculateNormal(vertices,indices,neigh) ;

    double area = calculateArea(vertices,indices,base) ;
     area += calculateArea(vertices,indices,neigh) ;

    double cos = glm::dot(baseNormal,neighNormal);
    glm::dvec3 t = glm::cross(baseNormal,neighNormal) ;
    double sin = glm::dot(intersectEdge,glm::normalize(t)) ;


    // signed angle in [-pi,pi]
    if (sin!=0.0 || cos!=0.0)
    {
        result    = glm::length(t)* atan2(sin,cos);
    }
    else
    {
        result = 0.0;
    }
    // add weighted Hf to scalar at v_l and v_r
    if (area!=0.0)
    {
        (result /= area) *=3.0;
    }

    return result ;

}



inline std::vector<double>* calculateMeanCurvature(std::vector<Vertex>& vertices,std::vector<unsigned int >& indices,std::vector<std::vector<unsigned int>>& point2Triangle)
{

    std::vector<double>* result =new std::vector<double>(vertices.size(),0.0) ;
    std::vector<int> resultsize(vertices.size(),0.0) ;
    int base = 0;
    for(auto indice =indices.begin();indice!=indices.end();)
    {
        glm::i32vec3 index ;
        glm::vec3 first = vertices[*indice].Position ;
        index[0] = *indice ;
        indice++ ;

        glm::vec3 second = vertices[*indice].Position ;
        index[1] = *indice ;
        indice++ ;

        glm::vec3 third  = vertices[*indice].Position ;
        index[2] = *indice ;
        indice++ ;

        glm::dvec3 f_s = second - first ;
        glm::dvec3 s_t = third - second ;
        glm::dvec3 t_f = first - third ;

        unsigned int f_s_neighborTriIndex = getNeighborTriangleIndex(base,index[0],index[1],point2Triangle) ;
        unsigned int s_t_neighborTriIndex = getNeighborTriangleIndex(base,index[1],index[2],point2Triangle) ;
        unsigned int t_f_neighborTriIndex = getNeighborTriangleIndex(base,index[2],index[0],point2Triangle) ;

        double f_s_hf = calculateAngleCur(vertices,indices,base,f_s_neighborTriIndex,f_s) ;
        double s_t_hf = calculateAngleCur(vertices,indices,base,s_t_neighborTriIndex,s_t) ;
        double t_f_hf = calculateAngleCur(vertices,indices,base,t_f_neighborTriIndex,t_f) ;

        (*result)[index[0]] += (f_s_hf + t_f_hf) ;
        (*result)[index[1]] += (s_t_hf + f_s_hf) ;
        (*result)[index[2]] += (t_f_hf + s_t_hf) ;



        resultsize[index[0]] += 2;
        resultsize[index[1]] += 2;
        resultsize[index[2]] += 2;


        base++ ;

    }

    for(unsigned int i  =0 ; i < result->size();i++)
    {
        if(resultsize[i] > 0)
        {
            (*result)[i] = 0.5*(*result)[i] /double(resultsize[i]) ;
        }
    }
    return result ;

}

 inline std::vector<double>* calculateGaussCurvature(std::vector<Vertex>& vertices,std::vector<unsigned int >& indices,std::vector<std::vector<unsigned int>>& point2Triangle)
{

    std::vector<double>* result =new std::vector<double>(vertices.size(),0.0) ;
    unsigned int verticeNum = vertices.size() ;
    double* K = new double[verticeNum];
    double* dA = new double[verticeNum];
    double pi2 = 2.0*AI_MATH_PI;

    for (int k = 0; k < verticeNum; k++)
    {
        K[k]  = pi2;
        dA[k] = 0.0;
    }

    glm::dvec3 v0, v1, v2, e0, e1, e2;

    double A, alpha0, alpha1, alpha2;

    for(unsigned int i = 0 ;i<indices.size()/3;i++ )
    {
        v0 = vertices[indices[i*3]].Position ;
        v1 = vertices[indices[i*3+1]].Position ;
        v2 = vertices[indices[i*3+2]].Position ;

        // edges
        e0[0] = v1[0] ; e0[1] = v1[1] ; e0[2] = v1[2] ;
        e0[0] -= v0[0]; e0[1] -= v0[1]; e0[2] -= v0[2];

        e1[0] = v2[0] ; e1[1] = v2[1] ; e1[2] = v2[2] ;
        e1[0] -= v1[0]; e1[1] -= v1[1]; e1[2] -= v1[2];

        e2[0] = v0[0] ; e2[1] = v0[1] ; e2[2] = v0[2] ;
        e2[0] -= v2[0]; e2[1] -= v2[1]; e2[2] -= v2[2];

        e0 = glm::normalize(e0);
        e1 = glm::normalize(e1) ;
        e2 = glm::normalize(e2) ;


        double ac1 = glm::dot(e1,e2);
        double ac2 = glm::dot(e2,e0);
        double ac3 = glm::dot(e0,e1);
        alpha0 = acos(-CLAMP_MACRO(ac1));
        alpha1 = acos(-CLAMP_MACRO(ac2));
        alpha2 = acos(-CLAMP_MACRO(ac3));

        // surf. area
        A = calculateArea(vertices,indices,i) ;
        // UPDATE
        dA[indices[i*3]] += A;
        dA[indices[i*3+1]] += A;
        dA[indices[i*3+2]] += A;
        K[indices[i*3]] -= alpha1;
        K[indices[i*3+1]] -= alpha2;
        K[indices[i*3+2]] -= alpha0;

    }

    for(unsigned int i =0; i < verticeNum;i++)
    {
        if (dA[i]>0.0)
        {
            (*result)[i] = 3.0*K[i]/dA[i];
        }
        else
        {
            (*result)[i] = 0.0;
        }

    }

    delete [] K;
    delete [] dA;
    return result ;
}


inline void  calculateAllMinCurvature( std::vector<Vertex>& vertices,std::vector<unsigned int >& indices )
{
    std::vector<std::vector<unsigned  int >> point2Triangle(vertices.size()) ;


    int base = 0;
    int flag = 0 ;
    for(auto indice = indices.begin() ;indice!=indices.end();indice++)
    {
        point2Triangle[(*indice)].push_back(base) ;
        flag++ ;
        if(flag%3==0)
        {
            flag = 0 ;
            base++ ;
        }
    }

    std::vector<double>* meanCurvature = calculateMeanCurvature(vertices,indices,point2Triangle) ;
    std::vector<double>* gaussCurvature = calculateGaussCurvature(vertices,indices,point2Triangle) ;

//    std::vector<double>* minCurvature = new std::vector<double>(vertices.size(),0.0)  ;

    double gauss, mean, res,tmp,max,min ;

    max = -1000 ;
    min = 1000 ;
    for(unsigned int i = 0; i< vertices.size();i++)
    {
        gauss = (*gaussCurvature)[i] ;
        mean = (*meanCurvature)[i];

        tmp = mean*mean - gauss;
        if (tmp >= 0)
        {
            res = mean - sqrt(tmp);
        }else{
            res = 0 ;
        }


        if(res<-5)
        {
            res = -5 ;
        }

        if(res>0.2)
        {
            res = 0.2;
        }

        res = (res + 5)/(5.2) ;

       // res = (exp(res)-1)/exp(res) ;

        vertices[i].curvature = res ;
        if(res>max)
        {
            max = res ;
        }
        if(res<min)
        {
            min = res ;
        }
//        (*minCurvature)[i] = res ;
    }

    std::cout<<"Max Curvature is "<<std::to_string(max)<<std::endl ;
    std::cout<<"Min Curvature is "<<std::to_string(min)<<std::endl ;

    delete meanCurvature ;
    delete gaussCurvature ;
//    return minCurvature;


}


#endif //LEARNOPENGL_CALCUATECURVATURE_H
