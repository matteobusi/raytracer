/* 
 * File:   Sphere.cpp
 * Author: caos
 * 
 * Created on June 18, 2015, 12:22 PM
 */

#include "Sphere.h"

bool Sphere::hit(Ray& r, float& t, Surface** who, Vector& normal, Point& intersect)
{
    *who = this;
    float D = delta(r);
    if(D<0) 
        return false;
    else
    {
        D=sqrt(D);
        Vector d = r.getDirection();
        Vector diff = Vector::add(r.getOrigin(), Vector::multiply(center, -1));  


        float sq_d = Vector::dot_product(d, d);        
        float prod = Vector::dot_product(d, diff);
        
        float t1 = (-prod + D)/sq_d;
        float t2 = (-prod - D)/sq_d;

        Vector e = r.getOrigin();    
        if(abs(t1) >= abs(t2))
        {
            intersect = Vector::add(e, Vector::multiply(d, t2));
            t = t2;
        }
        else if(abs(t1) < abs(t2))
        {
             intersect = Vector::add(e, Vector::multiply(d, t1));
             t = t1;
        }
        
        normal = Vector::multiply(Vector::add(intersect, Vector::multiply(center, -1)), 1.f/radius);
        return true;
    }
}

bool Sphere::isHit(Ray& r)
{
    return (delta(r) >= 0);
}


float Sphere::delta(Ray& r)
{
    Vector d = r.getDirection();
    
    Vector diff = Vector::add(r.getOrigin(), Vector::multiply(center, -1));  

    float sq_prod = Vector::dot_product(d, diff);
    sq_prod *= sq_prod;
    
    float sq_d = Vector::dot_product(d, d);
    float sq_diff = Vector::dot_product(diff, diff) - radius*radius;
    
    return (sq_prod - sq_diff*sq_d);    
}
