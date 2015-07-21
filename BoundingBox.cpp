/* 
 * File:   BoundingBox.cpp
 * Author: caos
 * 
 * Created on July 1, 2015, 8:23 PM
 */

#include "BoundingBox.h"

bool BoundingBox::isHit(const Ray& r)  const 
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;
    
    Vector rayDir = r.getDirection();
    Point rayOrigin = r.getOrigin();
    
    if(rayDir.getX() >= 0)
    {
        tmin = (points[0].getX()-rayOrigin.getX())/rayDir.getX();        
        tmax = (points[1].getX()-rayOrigin.getX())/rayDir.getX();
    }
    else
    {
        tmin = (points[1].getX()-rayOrigin.getX())/rayDir.getX();        
        tmax = (points[0].getX()-rayOrigin.getX())/rayDir.getX();
    }
    
    if(rayDir.getY() >= 0)
    {
        tymin = (points[0].getY()-rayOrigin.getY())/rayDir.getY();        
        tymax = (points[1].getY()-rayOrigin.getY())/rayDir.getY();
    }
    else
    {
        tymin = (points[1].getY()-rayOrigin.getY())/rayDir.getY();        
        tymax = (points[0].getY()-rayOrigin.getY())/rayDir.getY();
    }
    
    if( (tmin > tymax) || (tymin > tmax) ) return false;
    
    if(tymin > tmin) tmin = tymin;
    if(tymax < tmax) tmax = tymax;
    
    if(rayDir.getZ() >= 0)
    {
        tzmin = (points[0].getZ()-rayOrigin.getZ())/rayDir.getZ();        
        tzmax = (points[1].getZ()-rayOrigin.getZ())/rayDir.getZ();
    }
    else
    {        
        tzmin = (points[1].getZ()-rayOrigin.getZ())/rayDir.getZ();        
        tzmax = (points[0].getZ()-rayOrigin.getZ())/rayDir.getZ();
    }
    
    if( (tmin > tzmax) || (tzmin > tmax) ) return false;
    
    if(tzmin > tmin) tmin = tzmin;
    if(tzmax < tmax) tmax = tzmax;
    
    return true;
}