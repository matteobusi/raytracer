/* 
 * File:   BVH.h
 * Author: caos
 *
 * Created on July 1, 2015, 8:02 PM
 */

#ifndef BVH_H
#define	BVH_H

#include <vector>
#include <stdexcept>
#include <algorithm>

#include "BoundingBox.h"
#include "Surface.h"
#include "Material.h"

class BVH : Surface
{
public:
    BVH() {};
    BVH(std::vector<Surface*> scene, int axis, int l, int r);

    bool hit(Ray& r, float& t, Surface** who, Vector& normal, Point& intersect);
    
    bool isHit(Ray& r)
    {
        float t;
        Vector normal;
        Point p;
        Surface* who;
        return hit(r, t, &who, normal, p);
    };
    
    Surface* clone()
    { 
        BVH* tmp = new BVH();
        tmp->left = left;
        tmp->right = right;
        return tmp;
    };
    
    Material getMaterial() { throw std::runtime_error("Shouldn't be calling getMaterial() on BVH!"); };
    BoundingBox getBoundingBox(){ return boundingBox; };

    ~BVH();
private:
    Surface* left, *right;
    BoundingBox boundingBox; 
    static int qsplit(std::vector<Surface*>& scene, float pivotComp, int axis, int l, int r);
    static float getComponent(const Vector& v, int axis)
    {
        if(axis==0) return v.getX();
        else if(axis == 1) return v.getY();
        return v.getZ();
    };
};

#endif	/* BVH_H */

