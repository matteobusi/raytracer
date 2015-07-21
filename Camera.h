#ifndef CAMERA_H
#define	CAMERA_H

#include "Vector.h"

class Camera
{
public:    
    Camera(const Point& _e, const Point& _viewdir, const Point& _up, float _d, float _l, float _r, float _b, float _t) : e(_e), d(_d), l(_l), r(_r), b(_b), t(_t) 
    {
        w = Vector::multiply(_viewdir, -1.f/_viewdir.euclidean_norm());
        Vector prod = Vector::cross_product(_up, w);
        u = Vector::multiply(prod, 1.f/prod.euclidean_norm());
        v = Vector::cross_product(w, u);
    };
    
    Camera() {};
    Camera(const Camera& o) : e(o.e), u(o.u), w(o.w), v(o.v), d(o.d), l(o.l), r(o.r), b(o.b), t(o.t) { };
    
    /* Standard getters */
    const Point& getViewpoint() const;
    const Vector& getU() const;
    const Vector& getW() const;
    const Vector& getV() const;
    float getFocalLength() const;
    
    float getL() const { return l; };
    float getR() const { return r; };
    float getB() const { return b; };
    float getT() const { return t; };
    
private:
    /* Camera viewpoint */
    Point e; 
    
    /* Basis for the coordinate system */
    Vector u, w, v;   
    
    /* Camera focal length */
    float d;
    
    /* Image plane */
    float l,r,b,t; 
};

#endif	/* CAMERA_H */

