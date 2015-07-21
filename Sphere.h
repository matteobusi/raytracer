/* 
 * File:   Sphere.h
 * Author: caos
 *
 * Created on June 18, 2015, 12:22 PM
 */

#ifndef SPHERE_H
#define	SPHERE_H

#include <cstdlib>
#include <cmath>

#include "Surface.h"
#include "Ray.h"
#include "Vector.h"
#include "Vector.h"
#include "Color.h"
#include "BoundingBox.h"

class Sphere : public Surface
{
public:
    Sphere(float _radius, const Point& _center, const Material& _material) \
            : radius(_radius), center(_center), material(_material) { bb = NULL; };
    Sphere(const Sphere& s) : radius(s.radius), center(s.center), material(s.material), bb(s.bb) {};
    bool hit(Ray& r, float& t, Surface** who, Vector& normal, Point& intersect);
    bool isHit(Ray& r); 

    Material getMaterial() { return material; };   
    
    virtual BoundingBox getBoundingBox()
    { 
        if(bb==NULL)
        {
            Vector rad = Vector(radius, radius, radius);
            
            Vector maxV = Vector::add(center, rad);
            
            rad = Vector::multiply(rad, -1.f);
            Vector minV = Vector::add(center, rad);
            
            bb = new BoundingBox(minV, maxV);
        }
        return *bb;
    };
        
    virtual Sphere* clone()
    {
        Sphere* s = new Sphere(radius, center, material);
        return s;
    };
    
    virtual ~Sphere() 
    {
        if(bb!=NULL) delete bb;
    };
    
private:
    float radius;
    Point center;
           
    Material material;
    
    BoundingBox* bb;   
    
    float delta(Ray& r);
};

#endif	/* SPHERE_H */

