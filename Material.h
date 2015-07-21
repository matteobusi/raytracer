/* 
 * File:   Material.h
 * Author: caos
 *
 * Created on June 20, 2015, 4:18 PM
 */

#ifndef MATERIAL_H
#define	MATERIAL_H
#include "Color.h"
class Material
{
public:
    Material (Color _surfaceColor = Color(), Color _specularColor = Color(),  Color _ambientColor = Color(),
              float _phongExp = 50.f, 
              bool _specular = false, 
              bool _dielectric = false, float _refractiveIndex = 1.f)\
            : surfaceColor(_surfaceColor), specularColor(_specularColor), ambientColor(_ambientColor), 
              phongExp(_phongExp), 
              specular(_specular), 
              dielectric(_dielectric), refractiveIndex(_refractiveIndex) {};
    Material(const Material& orig)\
            : surfaceColor(orig.surfaceColor), specularColor(orig.specularColor), ambientColor(orig.ambientColor),
              phongExp(orig.phongExp), 
              specular(orig.specular),  
              dielectric(orig.dielectric), refractiveIndex(orig.refractiveIndex) {};
    
    Color getSpecularColor() const { return specularColor; };
    Color getSurfaceColor() const { return surfaceColor; };
    Color getAmbientColor() const { return ambientColor; };
    float getPhongExp() const { return phongExp; };
    float getRefractiveIndex() const { return refractiveIndex; };
    bool isSpecular() const { return specular; };
    bool isDielectric() const { return dielectric; };

private:        
    Color surfaceColor;
    Color specularColor;
    Color ambientColor;
    float phongExp;
    
    bool specular;   
    
    bool dielectric;
    float refractiveIndex;
};

#endif	/* MATERIAL_H */

