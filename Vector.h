/* 
 * File:   Vector.h
 * Author: caos
 *
 * Created on June 16, 2015, 4:54 PM
 */

#ifndef VECTOR_H
#define	VECTOR_H

#include <cmath>
#include <limits>

class Vector 
{
public:
    Vector() : x(0), y(0), z(0) {};
    Vector(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {} ;
    Vector(const Vector& o) : x(o.x), y(o.y), z(o.z) {};
    
    inline float getX() const { return x;} ;
    inline float getY() const { return y;} ;
    inline float getZ() const { return z;} ;   
    
    /* Given 3 points build the "minimum" vector */
    static Vector min(const Vector& a, const Vector& b)
    {
        return Vector(_min(a.x, b.x), _min(a.y, b.y), _min(a.z, b.z));
    };
    
    static Vector max(const Vector& a, const Vector& b)
    {
        return Vector(_max(a.x, b.x), _max(a.y, b.y), _max(a.z, b.z));
    };
    
    /* addition */
    static Vector add(const Vector& lhs, const Vector& rhs)
    {
        Vector res;
        
        res.x = lhs.x + rhs.x;
        res.y = lhs.y + rhs.y;
        res.z = lhs.z + rhs.z;
        
        return res; 
    };
    
    /* times a scalar */
    static Vector multiply(const Vector& lhs, float rhs)
    {
        Vector res(lhs);
        
        res.x *= rhs;
        res.y *= rhs;
        res.z *= rhs;
        
        return res;
    };

    static float dot_product(const Vector& lhs, const Vector& rhs)
    {
        float sum = lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
        return sum;
    };
        
    static Vector cross_product(const Vector& lhs, const Vector& rhs)
    {
        Vector res;
        
        res.z = lhs.x * rhs.y - lhs.y * rhs.x;
        res.x = lhs.y * rhs.z - lhs.z * rhs.y;      
        res.y = lhs.z * rhs.x - lhs.x * rhs.z;
        
        return res;
    };
        
    
    float euclidean_norm() const
    {
        return sqrt(x*x + y*y + z*z);
    };
    
    
private:
    float x, y, z;
    
    static float _min(float a, float b)
    {
        return (a<b)?a:b;
    }
    
    static float _max(float a, float b)
    {
        return (a>b)?a:b;
    }
};

typedef Vector Point;
#endif	/* VECTOR_H */

