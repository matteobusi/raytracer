/* 
 * File:   Ray.h
 * Author: caos
 *
 * Created on June 16, 2015, 3:26 PM
 */

#ifndef RAY_H
#define	RAY_H

#include "Camera.h"
#include "Vector.h"
#include "Vector.h"

class Ray
{
public:
    Ray(Camera& cam, float u, float v);
    Ray(const Point& _origin, const Vector& _direction) : origin(_origin), direction(_direction) {};
    Point getOrigin() const { return origin; };
    Vector getDirection() const { return direction; };
private:
    Point origin;
    Vector direction;
};

#endif	/* RAY_H */

