/* 
 * File:   Color.h
 * Author: caos
 *
 * Created on June 18, 2015, 11:48 PM
 */

#ifndef COLOR_H
#define	COLOR_H

#include <cstdlib>

class Color
{
public:
    Color(int _r, int _g, int _b) : r((_r>255)?255:_r), g((_g>255)?255:_g), b((_b>255)?255:_b) {} ;
    Color() 
    {
        r = rand()%256; 
        g = rand()%256;
        b = rand()%256; 
    };   
    int getR() { return r;} ;
    int getG() { return g;} ;
    int getB() { return b;} ;
    
    static Color multiply(const Color& lhs, float coeff) { return Color(lhs.r*coeff, lhs.g*coeff, lhs.b*coeff);};
    static Color multiply(const Color& lhs, const Color& rhs) 
    {
        Color res;
        
        res.r = (lhs.r*rhs.r)/255;
        res.g = (lhs.g*rhs.g)/255;
        res.b = (lhs.b*rhs.b)/255;
        return res;
    };
    static Color add(const Color& lhs, const Color& rhs) { return Color(lhs.r+rhs.r, lhs.g+rhs.g, lhs.b+rhs.b);};
private:
    int r, g, b;    
};

#endif	/* COLOR_H */

