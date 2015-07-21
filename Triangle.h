/* 
 * File:   Triangle.h
 * Author: caos
 *
 * Created on June 22, 2015, 12:25 PM
 */

#ifndef TRIANGLE_H
#define	TRIANGLE_H

#include "Surface.h"
#include "Ray.h"
#include "Vector.h"
#include "Vector.h"
#include "Color.h"
#include "BoundingBox.h"

class Triangle : public Surface
{
public:
    Triangle(const Point& a, const Point& b, const Point& c, const Material& _material) : material(_material)
    {
        vertices[0]=a;
        vertices[1]=b;
        vertices[2]=c;
        
        bb = NULL;
    };
    
    Triangle(const Triangle& t)
    {
        for(int i=0; i < 3; i++)
            vertices[i] = t.vertices[i];
        material = t.material;
        
        bb = t.bb;
    };
    
    bool hit(Ray& r, float& t, Surface** who, Vector& normal, Point& intersect);
    bool isHit(Ray& r) 
    {
        float t;
        Vector normal;
        Point p;
        Surface* tmp;
        return hit(r, t, &tmp, normal, p);
    }; 
    
    Material getMaterial() { return material; };
    virtual Triangle* clone() 
    {
        Triangle* t = new Triangle(vertices[0], vertices[1], vertices[2], material); 
        return t; 
    };
    
    virtual BoundingBox getBoundingBox()
    { 
        if(bb==NULL)
        {
            Vector minV = Vector::min(vertices[2], Vector::min(vertices[0], vertices[1]));
            Vector maxV = Vector::max(vertices[2], Vector::max(vertices[0], vertices[1]));

            
            bb = new BoundingBox(minV, maxV);
        }
        return *bb;
    };
    
    virtual ~Triangle() 
    {
        if(bb!=NULL) delete bb;
    };
    
private:
    BoundingBox* bb;
    Point vertices[3];
    Material material;
    static const float TRIANGLE_EPS = 0.001;
    
    Point getCenter();
    float getRadius();
    
};

#endif	/* TRIANGLE_H */

