/* 
 * File:   Scene.cpp
 * Author: caos
 * 
 * Created on June 16, 2015, 4:56 PM
 */

#include "Scene.h"

#include <iostream>

using namespace tinyobj;

Scene::Scene(const Camera& _camera, const Color& _backgroundColor, const std::string fileName) : backgroundColor(_backgroundColor)
{
    finalized = false;

    camera = _camera;
    backgroundColor = _backgroundColor;
    
    std::vector<shape_t> shapes;
    std::vector<material_t> materials;
    
    #ifdef _WIN32
        int idx = fileName.find_last_of("\\");
    #else
        int idx = fileName.find_last_of("/");
    #endif
    
    std::string err;
    if(idx > 0)    
    {
        std::string matPath = fileName.substr(0, idx+1);
        err = tinyobj::LoadObj(shapes, materials, fileName.c_str(), matPath.c_str());
    }
    else
        err = tinyobj::LoadObj(shapes, materials, fileName.c_str());
    
    if (!err.empty()) 
        throw std::invalid_argument(err);

    /* foreach shape */
    for(std::vector<shape_t>::iterator it = shapes.begin(); it != shapes.end(); ++it)
    {
        int count=0;
        Point lightPos(0, 0, 0);
        Color lightColor(0, 0, 0);
        
        /* Add triangles! */
        assert(((*it).mesh.indices.size() % 3) == 0);

        int shSz = (*it).mesh.indices.size()/3;
        for (int i = 0; i < shSz; i++)
	{
            int aIdx =  (*it).mesh.indices[3*i+0];
            int bIdx =  (*it).mesh.indices[3*i+1];
            int cIdx =  (*it).mesh.indices[3*i+2];
            int matIdx = (*it).mesh.material_ids[i];
            
            Point a(-(*it).mesh.positions[3*aIdx+0], (*it).mesh.positions[3*aIdx+1], (*it).mesh.positions[3*aIdx+2]);            
            Point b(-(*it).mesh.positions[3*bIdx+0], (*it).mesh.positions[3*bIdx+1], (*it).mesh.positions[3*bIdx+2]);            
            Point c(-(*it).mesh.positions[3*cIdx+0], (*it).mesh.positions[3*cIdx+1], (*it).mesh.positions[3*cIdx+2]);

            material_t mat = materials[matIdx];
            
            Color ambientColor(255*mat.ambient[0], 255*mat.ambient[1], 255*mat.ambient[2]);            
            Color specularColor(255*mat.specular[0], 255*mat.specular[1], 255*mat.specular[2]);            
            Color surfaceColor(255*mat.diffuse[0], 255*mat.diffuse[1], 255*mat.diffuse[2]);
            Color emittingColor(255*mat.emission[0], 255*mat.emission[1], 255*mat.emission[2]);
            
            float phongExp = mat.shininess;
            float refractiveIndex = mat.ior;
            
            bool specular = (mat.illum >= 3 && mat.illum <= 9);
            bool dielectric = (mat.illum >= 4 && mat.illum <=9 && mat.illum!=8 && mat.illum!=5);
            
            if(emittingColor.getB() != 0 && emittingColor.getG() != 0 && emittingColor.getR() != 0)
            {
                lightPos = Vector::add(lightPos, Vector::add(a, Vector::add(b, c)));
                lightColor = Color::add(lightColor, emittingColor);
                count += 3;
            }
            else
            {
                Material trMat(surfaceColor, specularColor, ambientColor, phongExp, specular, dielectric, refractiveIndex);            
                Triangle tmp(a, b, c, trMat);            
                addSurface(tmp);
            }
        }
        
        if(count > 0)
        {
            lightPos = Vector::multiply(lightPos, 1.f/count);
            Light l(lightPos, lightColor, 1.f);
            addLight(l);
        }
    }
}

void Scene::addLight(Light& light)
{
    lights.push_back(light);
}

void Scene::addSurface(Surface& surf)
{    
    finalized = false;
    Surface* tmp = surf.clone();
    objects.push_back(tmp);  
}

void Scene::renderScene(Image& dest, int aaFactor)
{    
    srand(time(NULL));
    int w = dest.getW();
    int h = dest.getH();
    
    float l = camera.getL();
    float b = camera.getB();
    
    /* Render the scene! */            
    float uTmp = (camera.getR()-l)/w;
    float vTmp = (camera.getT()-b)/h;  
    
    /* Variables for the near and far plane! */
    float tmin = 0.001;
    float tmax = std::numeric_limits<float>::infinity();

    float red,green,blue;
    for(int i=0; i < w; i++)
        for(int j=0; j < h; j++)
        {
            red=green=blue=0;          
            if(aaFactor)
            {
                for(int p=0; p < aaFactor; p++)
                    for(int q = 0; q < aaFactor; q++)
                    {
                        float xi = ((float)rand())/RAND_MAX;
                        float u = l + (i+(p+xi)/aaFactor)*uTmp;
                        float v = b + (j+(q+xi)/aaFactor)*vTmp;

                        Ray vRay(camera, u, v);   
                        Color tmp = getColor(vRay, tmin, tmax);
                        red += tmp.getR();
                        green += tmp.getG();
                        blue += tmp.getB();
                    }
                    int sq = aaFactor*aaFactor;
                    Color final(red/sq, green/sq, blue/sq);
                    dest.set(i, j, final);
            }
            else
            {
                float u = l + (i+0.5)*uTmp;
                float v = b + (j+0.5)*vTmp;

                Ray vRay(camera, u, v);                 
                dest.set(i, j, getColor(vRay, tmin, tmax));
            }            
        }
}


inline bool Scene::findNearestSurface(Ray& ray, float tmin, float tmax, Surface** who, Vector& norm, Point& intersectionPoint, float& t)
{    
    if(!finalized)
        throw std::invalid_argument("Non-finalized scene! Did you forget to call finalizeScene()?");
    bool hit = sceneTree->hit(ray, t, who, norm, intersectionPoint);
    hit = hit && tmin <= t && tmax >= t;
    return hit;
}

inline Color Scene::getAmbient(const Material& material)
{
    return Color::multiply(material.getAmbientColor(), 0.2f);
}

inline Color Scene::getLambertDiffusion(const Material& material, const Light& light, const Vector& l, const Vector& norm)
{
     /* Diffusion by Lambert */
    float M = Vector::dot_product(norm, l);
    if(M < 0) 
        return Color(0,0,0);
    
    Color diffusion = Color::multiply(Color::multiply(material.getSurfaceColor(), light.getColor()), light.getIntensity());
    diffusion = Color::multiply(diffusion, M);

    return diffusion;
}
inline Color Scene::getPhongBlinnSpecular(const Material& material, const Light& light, const Vector& w, const Vector& l, const Vector& norm)
{
    /* And now Phong-Blinn */
    Vector h = Vector::add(w, l);
    h = Vector::multiply(h, 1.f/h.euclidean_norm());
    float M = Vector::dot_product(norm, h);
    if(M < 0)
        return Color(0,0,0);

    float phongCoeff = pow(M, material.getPhongExp());
    Color specular = Color::multiply(Color::multiply(material.getSpecularColor(), light.getColor()), phongCoeff);
    specular = Color::multiply(specular, M);
    
    return specular;
}

inline Vector Scene::getReflection(const Vector& d, const Vector& n)
{
    float mult = -2*Vector::dot_product(d, n);
    return Vector::add(d, Vector::multiply(n, mult));
}

inline bool Scene::getRefraction(const Vector& d, const Vector& n, float index, Vector& t)
{
    float dn = Vector::dot_product(d, n);
    float dnsq = dn*dn;
    
    Vector add1 = Vector::add(d, Vector::multiply(n, -dn));
    add1 = Vector::multiply(add1, 1.f/index);
    
    float sqAdd2 = 1.f - (1.f-dnsq)/(index*index);
    if(sqAdd2<0) 
        return false;
    
    Vector add2 = Vector::multiply(n, -sqrt(sqAdd2));
    
    t = Vector::add(add1, add2);
    return true;
}

Color Scene::getColor(Ray& ray, float tmin, float tmax, int recur)
{
    Surface* who;
    Vector hitN;
    Point hitP;
    float hitt;

    bool hit = findNearestSurface(ray, tmin, tmax, &who, hitN, hitP, hitt);
    
    if(!hit)
        return backgroundColor;
    else
    {
        Material surfaceMaterial = who->getMaterial();
        /* Ambient shading */
        Color L = getAmbient(surfaceMaterial);

        /* Shades */
        /* For each light in scene */
        for(std::vector<Light>::iterator it = lights.begin(); it != lights.end(); ++it) 
        {
            /* Light direction */
            Vector l = Vector::add(it->getPosition(), Vector::multiply(hitP, -1.f));
            Ray sRay(hitP, l);
            Vector dummyShadowNorm;
            Point dummyShadowPoint;
            Surface* dummyShadowSurface;
            float s=0;

            bool isInShadow = findNearestSurface(sRay, tmin, tmax, &dummyShadowSurface, dummyShadowNorm, dummyShadowPoint, s) ;
                        
            float dl = l.euclidean_norm();
            float ol = Vector::multiply(l, s).euclidean_norm();
                             
            isInShadow = isInShadow && s >= EPS && ol < dl; /* Check the light is not between hitpoint and hit object */
            
            if(!isInShadow)
            {
                l = Vector::multiply(l, 1.f/l.euclidean_norm());
                Color diffusion = Color::multiply(getLambertDiffusion(surfaceMaterial, *it, l, hitN), it->getIntensity());
                Color specular = Color::multiply(getPhongBlinnSpecular(surfaceMaterial, *it, camera.getW(),l, hitN), it->getIntensity());
                L = Color::add(L, Color::add(specular, diffusion));
            }   
            
            if(surfaceMaterial.isDielectric() && recur <= REC_MAX)
            {        
                float index = surfaceMaterial.getRefractiveIndex();
                hitN = Vector::multiply(hitN, 1.f/hitN.euclidean_norm());            
                Vector direction = ray.getDirection();
                direction = Vector::multiply(direction, 1.f/direction.euclidean_norm());

                float cosTetha;
                Vector refractionDir;            
                Vector reflectionDir = getReflection(direction, hitN);

                Ray reflectionRay(hitP, reflectionDir);

                float dn=Vector::dot_product(direction, hitN);
                float a;
                if(dn < 0) 
                {
                    getRefraction(direction, hitN, index, refractionDir); 
                    cosTetha = -dn;
                    a=0.5f;
                }
                else
                {
                    /* We are searching for the "next" hit and we compute the distance*/
                    Ray tmpRay(hitP, refractionDir);
                    Surface* dummyWho;
                    Vector dummyN;
                    Point dummyP;
                    float nextt = 0.f;
                    
                    findNearestSurface(tmpRay, tmin, tmax, &dummyWho, dummyN, dummyP, nextt);
                    a = 0.8f*exp(-0.8*nextt);

                    if(getRefraction(direction, Vector::multiply(hitN, -1.f), 1.f/index, refractionDir))
                        cosTetha = Vector::dot_product(refractionDir, hitN);
                    else
                        return Color::add(L, Color::multiply(getColor(reflectionRay, tmin, tmax, recur+1), a));
                }

                float R0 = (index -1)/(index + 1);
                R0 *= R0;

                float R = R0 + (1-R0)*pow(1-cosTetha, 5.f);

                Ray refractionRay(hitP, refractionDir);

                Color reflectColor = getColor(reflectionRay, tmin, tmax, recur+1);
                Color refractColor = getColor(refractionRay, tmin, tmax, recur+1);

                reflectColor = Color::multiply(reflectColor, R);
                refractColor = Color::multiply(refractColor, index*(1.f - R));
                L = Color::add(L, Color::multiply(Color::add(reflectColor, refractColor), a));
            }
                                
            if(surfaceMaterial.isSpecular() && recur <= REC_MAX)
            {  
                Vector r = getReflection(ray.getDirection(), hitN);
                Ray nRay(hitP, r);

                Color reflect = getColor(nRay, tmin, tmax, recur+1);
                reflect = Color::multiply(reflect, surfaceMaterial.getSpecularColor());
                L = Color::add(L, reflect);
            }  
        }        

        return L;
    }
}

void Scene::finalizeScene()
{
    /* Build the tree! */  
    sceneTree = new BVH(objects, 0, 0, objects.size()-1);   
    finalized = true;
}

Scene::~Scene()
{
    for(std::vector<Surface*>::iterator it = objects.begin(); it != objects.end(); ++it) 
        delete *it;
    
    delete sceneTree;
}