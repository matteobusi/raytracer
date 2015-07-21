/* 
 * File:   Image.h
 * Author: caos
 *
 * Created on June 19, 2015, 12:28 AM
 */

#ifndef IMAGE_H
#define	IMAGE_H

#include <stdexcept>
#include <ctime>
#include <cstdlib>
#include <fstream>

#include "Color.h"

class Image
{
public:
    Image(int _w, int _h);
    
    void set(int x, int y, Color c);
    Color get(int x, int y);
    
    int getW() { return w;};
    int getH() { return h;};
    
    void writeToFile(std::string fileName);
    virtual ~Image();
private:
    int w, h;
    Color* pixels;
    
};

#endif	/* IMAGE_H */

