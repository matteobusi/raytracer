/* 
 * File:   Image.cpp
 * Author: caos
 * 
 * Created on June 19, 2015, 12:28 AM
 */

#include "Image.h"

Image::Image(int _w, int _h) : w(_w), h(_h)
{
    srand(time(NULL));
    pixels = new Color[w*h];
}

void Image::set(int x, int y, Color c)
{    
    if(x >= 0 && x < w && y>=0 && y < h)
        pixels[y*w + x] = c;
    else
        throw std::invalid_argument("[Image::set] index out of bound!");
}

Color Image::get(int x, int y)
{
    if(x >= 0 && x < w && y>=0 && y < h)
        return pixels[y*w + x];
    else
        throw std::invalid_argument("[Image::get] index out of bound!");
}

void Image::writeToFile(std::string fileName)
{
    /* Write the header */
    std::ofstream out(fileName.c_str());
    
    out << "P3" << std::endl;
    out << w << std::endl;
    out << h << std::endl;
    out << "255" << std::endl;
    
    Color col;
    for(int i=0; i < h; i++)
        for(int j=0; j < w; j++)
        {
            col = pixels[i*w + j];
            out << col.getR() << " " << col.getG() << " " << col.getB() <<  std::endl;
        }
    
    out.close();
}

Image::~Image()
{
    delete[] pixels;
}

