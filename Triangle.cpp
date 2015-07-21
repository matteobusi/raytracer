/* 
 * File:   Triangle.cpp
 * Author: caos
 * 
 * Created on June 22, 2015, 12:25 PM
 */

#include "Triangle.h"

bool Triangle::hit(Ray& r, float& t, Surface** who, Vector& normal, Point& intersect)
{
    *who = this;
    /* Implementing Moller-Trumbore algorithm */
    float det, invDet, beta, gamma;
    
    Vector minus0 = Vector::multiply(vertices[0], -1);
    Vector e1 = Vector::add(vertices[1], minus0);
    Vector e2 = Vector::add(vertices[2], minus0);
 
    Point p = Vector::cross_product(r.getDirection(), e2);
    det = Vector::dot_product(e1, p);
    
    if(det > -TRIANGLE_EPS && det < TRIANGLE_EPS) return false;
    
    invDet = 1.f/det;
    
    Vector T = Vector::add(r.getOrigin(), minus0);
    
    gamma = Vector::dot_product(T, p) * invDet;
    
    if(gamma < 0.f || gamma > 1.f) return false;
    
    Vector Q = Vector::cross_product(T, e1);
    beta = Vector::dot_product(r.getDirection(), Q) * invDet;
    if(beta < 0.f || beta + gamma > 1.f) return false;
    
    t = Vector::dot_product(e2, Q) * invDet;
    
    if(t > TRIANGLE_EPS) 
    {
        normal = Vector::cross_product(Vector::add(vertices[2], minus0), Vector::add(vertices[1], minus0));
         /* If the dot product (aka cos(T)) <= 0 then we have vectors in different directions, else the direction is the same */
        float multiplier = (Vector::dot_product(normal, r.getDirection())>=0)?(-1.f):(1.f);
        
        float normInv = multiplier/normal.euclidean_norm();
        normal = Vector::multiply(normal, normInv);
        intersect = Vector::add(r.getOrigin(), Vector::multiply(r.getDirection(), t));
        return true;
    }
    
    return false;
}