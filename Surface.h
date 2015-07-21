/* 
 * File:   Surface.h
 * Author: caos
 *
 * Created on June 16, 2015, 3:41 PM
 */

#ifndef SURFACE_H
#define	SURFACE_H

#include "Vector.h"
#include "Ray.h"
#include "Material.h"

#include "BoundingBox.h"

/*
    Interface defining informations about possible surfaces (aka objects) in a scene 
 */
class Surface
{
public:
    virtual bool hit(Ray& r, float& t, Surface** who, Vector& normal, Point& intersect) = 0;
    virtual bool isHit(Ray& r) = 0; 
    virtual Material getMaterial() = 0;
    virtual Surface* clone() = 0; 
    
    virtual BoundingBox getBoundingBox() = 0;
    
    virtual ~Surface() {};
private:
};

#endif	/* SURFACE_H */

