/* 
 * File:   Light.h
 * Author: caos
 *
 * Created on June 16, 2015, 4:56 PM
 */

#ifndef LIGHT_H
#define	LIGHT_H

#include "Vector.h"
#include "Color.h"

class Light
{
public:
    Light(const Point& _position, const Color& _color, float _intensity) : position(_position), color(_color), intensity(_intensity) {};
    Light(const Light& l) : position(l.position), color(l.color), intensity(l.intensity) {};
    Point getPosition() const { return position; };
    Color getColor() const { return color; };
    float getIntensity() const { return intensity; };
private:
    Point position;
    Color color;
    float intensity;
};

#endif	/* LIGHT_H */

