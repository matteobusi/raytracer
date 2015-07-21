/* 
 * File:   Camera.cpp
 * Author: caos
 * 
 * Created on June 16, 2015, 5:17 PM
 */

#include "Camera.h"

const Point& Camera::getViewpoint() const
{
    return e;
}

const Vector& Camera::getU() const 
{
    return u;
}

const Vector& Camera::getW() const 
{
    return w;
}

const Vector& Camera::getV() const
{
    return v;
}

float Camera::getFocalLength() const
{
    return d;
}