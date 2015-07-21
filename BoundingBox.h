/* 
 * File:   BoundingBox.h
 * Author: caos
 *
 * Created on July 1, 2015, 8:23 PM
 */

#ifndef BOUNDINGBOX_H
#define	BOUNDINGBOX_H

#include "Ray.h"
#include "Material.h"

class Surface;

class BoundingBox
{
public:
    BoundingBox() {} ;
    BoundingBox(const Vector& min, const Vector& max)
    {
        points[0] = min;
        points[1] = max;
    };    
    
    static inline BoundingBox combine(const BoundingBox& b1, const BoundingBox& b2)
    {
        Vector minV = Vector::min(b1.getMin(), b2.getMin());
        Vector maxV = Vector::max(b1.getMax(), b2.getMax());
        
        return BoundingBox(minV, maxV);
    };
    
    Vector getMin() const { return points[0]; };
    Vector getMax() const { return points[1]; };
    
    bool isHit(const Ray& r) const;    
    virtual ~BoundingBox() { };
    
private:  
    Vector points[2];    
    static const float EPSILON = 0.001;
};

#endif	/* BOUNDINGBOX_H */

