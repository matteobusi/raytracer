/* 
 * File:   Scene.h
 * Author: caos
 *
 * Created on June 16, 2015, 4:56 PM
 */

#ifndef SCENE_H
#define	SCENE_H

#include <vector>
#include <limits>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cmath>

#include "Light.h"
#include "Camera.h"
#include "Sphere.h"
#include "Image.h"
#include "Material.h"
#include "Triangle.h"
#include "BVH.h"

#include "tiny_obj_loader.h"

#include <cassert> 

class Scene 
{
public:
    Scene(const Camera& _camera, const Color& _backgroundColor) \
    : finalized(false), camera(_camera), backgroundColor(_backgroundColor) {};    
    Scene(const Camera& _camera, const Color& _backgroundColor, const std::string fileName);
    
    void addLight(Light& ligth);
    void addSurface(Surface& surf);
    
    void finalizeScene();
    void renderScene(Image& dest, int aaFactor=0);
    
    int getNumSurfaces() { return objects.size(); };
    int getNumLights() { return lights.size(); };
    virtual ~Scene();
private:
    static const float EPS = 0.0001;
    static const int REC_MAX = 3;
    
    bool finalized;
    BVH* sceneTree;
    std::vector<Surface*> objects;
    std::vector<Light> lights;
    Camera camera;
    
    Color backgroundColor;
    
    Color getColor(Ray& ray, float tmin, float tmax, int recur=0);
    inline bool findNearestSurface(Ray& ray, float tmin, float tmax, Surface** who, Vector& norm, Point& intersectionPoint, float& t);
    inline Color getAmbient(const Material& material);
    inline Color getLambertDiffusion(const Material& material, const Light& light, const Vector& l, const Vector& norm);
    inline Color getPhongBlinnSpecular(const Material& material, const Light& light, const Vector& w, const Vector& l, const Vector& norm);
    inline Vector getReflection(const Vector& d, const Vector& n);
    inline bool getRefraction(const Vector& d, const Vector& n, float index, Vector& t);
};

#endif	/* SCENE_H */

