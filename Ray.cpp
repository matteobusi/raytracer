/* 
 * File:   Ray.cpp
 * Author: caos
 * 
 * Created on June 16, 2015, 3:26 PM
 */

#include "Ray.h"

Ray::Ray(Camera& cam, float u, float v) : origin(cam.getViewpoint()), direction()
{
    float minus_d = -cam.getFocalLength();
    
    Vector v1(Vector::multiply(cam.getW(), minus_d));
    Vector v2(Vector::multiply(cam.getU(), u));
    Vector v3(Vector::multiply(cam.getV(), v));
    
    direction = Vector::add(v1, Vector::add(v2, v3));
}

