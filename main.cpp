/* 
 * File:   main.cpp
 * Author: caos
 *
 * Created on June 16, 2015, 3:24 PM
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

#include "Triangle.h"
#include "Scene.h"

using namespace std;

/*
 * The main program can be called by command line.
 * Arguments are:
 
 *  -c <fileName> contains the camera parameters in a format specified below.
 *  -a <val> [Optional] antialiasing factor, an integer. If not specified is off.
 *  -w <w> image width
 *  -h <h> image height
 *  -o <filename> indicates the output file. The rusult will be a PPM image.
 *  -i <filename> an obj file containing the scene to be rendered. Lights are added automatically.
 
 File format for camera specifies:
 * position of camera, e <x y z>
 * look at point: v <x y z>
 * up vector, up: u <x y z>
 * focal length, l: d <value>
 * sensor width, w: w <value>
 * Other informations are gathered by size parameters
 */

void print_help()
{
    cout << 
         "The main program can be called by command line." << endl <<
         "Arguments are:" << endl <<
         endl <<
         "*  -c <fileName> contains the camera parameters in a format specified below." << endl << 
         "*  -a <val> [Optional] antialiasing factor, an integer. If not specified is off." << endl <<
         "*  -w <w> image width" << endl << 
         "*  -h <h> image height" << endl <<
         "*  -o <filename> indicates the output file. The rusult will be a PPM image." << endl <<
         "*  -i <filename> an obj file containing the scene to be rendered. Lights are added automatically." << endl <<
         endl << 
         "File format for camera specifies:" << endl <<
         "* position of camera, e <x y z>" << endl <<
         "* look at point: v <x y z>" << endl <<
         "* up vector, up: u <x y z>" << endl << 
         "* focal length, l: d <value>" << endl <<
         "* sensor width, w: w <value>" << endl << 
         "* Other informations are gathered by size parameters" << endl;
}

int main(int argc, char** argv)
{
    if(argc <= 1) 
    {
        cout << "Usage: " << endl;
        print_help();
        exit(0);
    }
    
    /* Camera parameters */
    Point e;
    Vector v;
    Vector u;
    float d;
    
    /* rendering parameters: antialiasing, size, input and output */
    int a = 0;
    int w, h;
    float ratio, sensorWidth;
    
    string output, input;
    
    for(int i=1; i < argc; i++)
    {        
        if(strlen(argv[i]) > 1) /* a -, followed by a letter*/
        {
            switch(argv[i][1]) 
            {
            case 'c':
            {
                /* Load camera parameters from file,  */  
                ++i;
                ifstream in(argv[i]);
                
                while (true) 
                {
                    char c;
                    float x, y, z;
                    
                    in >> c;
                    if(in.eof())
                        break;
                                       
                    switch(c)
                    {
                        case 'd':   
                            in >> d;
                        break;
                        case 'w':
                            in >> sensorWidth;
                        break;
                        case 'e':
                            in >> x >> y >> z;
                            e = Point(x, y, z);
                            break;
                        case 'v':
                            in >> x >> y >> z;
                            v = Vector(x, y, z);
                            break;
                        case 'u':
                            in >> x >> y >> z;
                            u = Vector(x, y, z);
                            break;
                        default:
                            cerr << "Wrong camera file format!" << endl;
                    }
                }
                in.close();
                break;
            }
            case 'a':
                ++i;
                istringstream(string(argv[i])) >> a;
                break;
            case 'w':
                ++i;
                istringstream(string(argv[i])) >> w;
                break;
            case 'h':
                ++i;
                istringstream(string(argv[i])) >> h;
                break;
            case 'o':
                ++i;
                output = string(argv[i]);
                break;
            case 'i':
                ++i;
                input = string(argv[i]);
                break;
            default:
                cout << "Unknown parameter:" << endl;
                print_help();
                exit(0);
                break;
            }
        }
    }
    
    ratio = ((float)w)/((float)h);
    
    /* Create the camera */
    float sensorHeight = sensorWidth/ratio;
    
    cout << "Input file: " << input << endl;
    cout << "Output file: " << output << endl;
    cout << "    Image size: " << w << "x" << h << endl;
    cout << "Rendering settigs: " << endl;
    cout << "    Anti-aliasing factor: " << a << endl;
    
    cout << "Camera loaded from file:" << endl;
    cout << "    e: (" << e.getX() << ", " << e.getY() << ", " << e.getZ() << ")" << endl; 
    cout << "    look at: (" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ")" << endl; 
    cout << "    up: (" << u.getX() << ", " << u.getY() << ", " << u.getZ() << ")" << endl; 
    cout << "    focal length: " << d << endl;
    cout << "    sensor width: " << sensorWidth << endl;
    cout << "    sensor ratio: " << ratio << endl;
    
    Vector dirVector = Vector::add(v, Vector::multiply(e, -1.f));
    Camera mainCamera(e, dirVector, u, d, -sensorWidth/2, sensorWidth/2, -sensorHeight/2, sensorHeight/2);
        
    /* Setup the scene */
    Scene scene(mainCamera, Color(66, 66, 66), input);

    /* Render the scene! */
    Image res(w, h);
   
    cout << "Scene: " << endl;
    cout << "    #Surfaces: " << scene.getNumSurfaces() << endl;
    cout << "    #Lights: " << scene.getNumLights() << endl;
       
    int t = clock();
    scene.finalizeScene();
    cout << "Finalizing scene took " << ((float)(clock() - t))/CLOCKS_PER_SEC  << " seconds." << endl;

    t = clock();
    if(a <= 0)
        scene.renderScene(res);
    else 
        scene.renderScene(res, a);
    t = clock() - t;
    
    cerr << "Rendering took " << ((float)(t))/CLOCKS_PER_SEC  << " seconds." << endl;
    res.writeToFile(output);

    return 0;
}