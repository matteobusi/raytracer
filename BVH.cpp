/* 
 * File:   BVH.cpp
 * Author: caos
 * 
 * Created on July 1, 2015, 8:02 PM
 */

#include "BVH.h"

BVH::BVH(std::vector<Surface*> scene, int axis, int l, int r)
{
    if(r < l) return;
    
    if(r-l <= 1) /* one o two elements */
    {
        left = scene[l]->clone();
        right = scene[r]->clone();
        boundingBox = BoundingBox::combine(left->getBoundingBox(), right->getBoundingBox());
    }
    else
    {
        //std::cerr << "L: " << l << " R: " << r << "\n";
        boundingBox = scene[l]->getBoundingBox();
        for(int i=l+1; i <= r; i++)
            boundingBox = BoundingBox::combine(boundingBox, scene[i]->getBoundingBox());
        Vector pivot = Vector::multiply(Vector::add(boundingBox.getMin(), boundingBox.getMax()), 0.5f);
        float pivotComp = getComponent(pivot, axis);
        int mid = qsplit(scene, pivotComp, axis, l, r);
        
        left = new BVH(scene, (axis+1)%3, l, mid-1);        
        right = new BVH(scene, (axis+1)%3, mid, r);                
    }
}

int BVH::qsplit(std::vector<Surface*>& scene, float pivotComp, int axis, int l, int r)
{
    BoundingBox boundingBox;
    float centroid;
    int ret = l;
    
    for(int i=l; i <= r; i++)
    {
        boundingBox = scene[i]->getBoundingBox();
        centroid = getComponent(Vector::multiply(Vector::add(boundingBox.getMin(), boundingBox.getMax()), 0.5f), axis);
        
        if(centroid < pivotComp)
        {
            Surface* tmp = scene[i];
            scene[i] = scene[ret];
            scene[ret] = tmp;
            ret++;
        }
    }
    
    if(ret == l || ret == r-1) ret = (r+l)/2;
    
    return ret;
}

bool BVH::hit(Ray& r, float& t, Surface** who, Vector& normal, Point& intersect)
{
    if(!boundingBox.isHit(r)) 
        return false;
    
    /* Left and right */
    float tLeft, tRight;
    Vector nLeft, nRight;
    Point pLeft, pRight;
    Surface *whoL, *whoR;
    bool leftHit = false, rightHit = false;
    
    leftHit = left->hit(r, tLeft, &whoL, nLeft, pLeft);
    rightHit = right->hit(r, tRight, &whoR, nRight, pRight);

    if(leftHit && rightHit)
    {
        if(tLeft < tRight)
        {
            t = tLeft;
            normal = nLeft;
            *who = whoL;
            intersect = pLeft;
        }
        else
        {
            t = tRight;
            normal = nRight;
            *who = whoR;
            intersect = pRight;                
        }
        return true;
    }
    else if(leftHit)
    {
        t = tLeft;
        normal = nLeft;
        intersect = pLeft;
        *who = whoL;
        return true;
    }
    else if(rightHit)
    {
        t = tRight;
        normal = nRight;
        intersect = pRight;   
        *who = whoR;
        return true;
    }
    else
        return false;
}

BVH::~BVH()
{
    delete left;
    delete right;
}