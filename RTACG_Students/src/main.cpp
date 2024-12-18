#include <iostream>
#include <algorithm>

#include "core/film.h"
#include "core/matrix4x4.h"
#include "core/ray.h"
#include "core/utils.h"
#include "core/scene.h"


#include "shapes/sphere.h"
#include "shapes/infiniteplan.h"

#include "cameras/ortographic.h"
#include "cameras/perspective.h"

#include "shaders/intersectionshader.h"
#include "shaders/depthshader.h"
#include "shaders/normalshader.h"
#include "shaders/whittedintegrator.h"



#include "materials/phong.h"
#include "materials/mirror.h"
#include "materials/glass.h"
#include "materials/emissive.h"

#include <chrono>

using namespace std::chrono;

typedef std::chrono::duration<double, std::milli> durationMs;

#define DELTA 0.01
#define KEY 0.4
#define Lwhite 10.0f
#define REINHARD true
#define GAMMA 0.5f

#include <math.h>


void buildSceneCornellBox(Camera*& cam, Film*& film,
    Scene myScene)
{
    /* **************************** */
    /* Declare and place the camera */
    /* **************************** */
    Matrix4x4 cameraToWorld = Matrix4x4::translate(Vector3D(0, 0, -3));
    double fovDegrees = 60;
    double fovRadians = Utils::degreesToRadians(fovDegrees);
    cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);

    /* ********* */
    /* Materials */
    /* ********* */
    Material* redDiffuse = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0, 0, 0), 100);
    Material* greenDiffuse = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0, 0, 0), 100);
    Material* greyDiffuse = new Phong(Vector3D(0.8, 0.8, 0.8), Vector3D(0, 0, 0), 100);      
    Material* blueGlossy_20 = new Phong(Vector3D(0.2, 0.3, 0.8), Vector3D(0.8, 0.8, 0.8), 20);
    Material* blueGlossy_80 = new Phong(Vector3D(0.2, 0.3, 0.8), Vector3D(0.8, 0.8, 0.8), 80);
    Material* cyandiffuse = new Phong(Vector3D(0.2, 0.8, 0.8), Vector3D(0, 0, 0), 100);

    //Task 5.3
    Material* mirror = new Mirror();
    //Task 5.4
    // Material* transmissive = new Glass(Vector3D(5.0 / 255, 237.0 / 255, 144.0 / 255), 0.717);
    //Material* transmissive = new Glass(sqrt(2.0) / 2.0); // 0,707106
    Material* transmissive = new Glass(0.7); 


    /* ******* */
    /* Objects */
    /* ******* */
    double offset = 3.0;
    Matrix4x4 idTransform;
    // Construct the Cornell Box
    Shape* leftPlan = new InfinitePlan(Vector3D(-offset - 1, 0, 0), Vector3D(1, 0, 0), redDiffuse);
    Shape* rightPlan = new InfinitePlan(Vector3D(offset + 1, 0, 0), Vector3D(-1, 0, 0), greenDiffuse);
    Shape* topPlan = new InfinitePlan(Vector3D(0, offset, 0), Vector3D(0, -1, 0), greyDiffuse);
    Shape* bottomPlan = new InfinitePlan(Vector3D(0, -offset, 0), Vector3D(0, 1, 0), greyDiffuse);
    Shape* backPlan = new InfinitePlan(Vector3D(0, 0, 3 * offset), Vector3D(0, 0, -1), greyDiffuse);

    myScene.AddObject(leftPlan);
    myScene.AddObject(rightPlan);
    myScene.AddObject(topPlan);
    myScene.AddObject(bottomPlan);
    myScene.AddObject(backPlan);


    // Place the Spheres and square inside the Cornell Box
    double radius = 1;         
    Matrix4x4 sphereTransform1;
    sphereTransform1 = Matrix4x4::translate(Vector3D(1.5, -offset + radius, 6));
    Shape* s1 = new Sphere(radius, sphereTransform1, blueGlossy_20); 

    Matrix4x4 sphereTransform2;
    sphereTransform2 = Matrix4x4::translate(Vector3D(-1.5, -offset + 3*radius, 4));
    //Shape* s2 = new Sphere(radius, sphereTransform2, blueGlossy_80);
    //Shape* s2 = new Sphere(radius, sphereTransform2, mirror);
    Shape* s2 = new Sphere(radius, sphereTransform2, transmissive);

    //Shape* square = new Square(Vector3D(offset + 0.999, -offset - 0.2, 3.0), Vector3D(0.0, 4.0, 0.0), Vector3D(0.0, 0.0, 2.0), Vector3D(-1.0, 0.0, 0.0), cyandiffuse);
    Shape* square = new Square(Vector3D(offset + 0.999, -offset-0.2, 3.0), Vector3D(0.0, 4.0, 0.0), Vector3D(0.0, 0.0, 2.0), Vector3D(-1.0, 0.0, 0.0), mirror);

    myScene.AddObject(s1);
    myScene.AddObject(s2);
    myScene.AddObject(square);

    PointLightSource* myPointLight = new PointLightSource(Vector3D(0, 2.5, 3.0), Vector3D(2.0));
    //PointLightSource* myPointLight_2 = new PointLightSource(Vector3D(0, 0.0, 4.0), Vector3D(1.0, 0, 0));
    //PointLightSource* myPointLight_2 = new PointLightSource(Vector3D(0, 0.0, 4.0), Vector3D(1.5));

    myScene.AddPointLight(myPointLight);
    //myScene.AddPointLight(myPointLight_2);

}


void buildSceneSphere(Camera*& cam, Film*& film,
    Scene myScene)
{
    /* **************************** */
      /* Declare and place the camera */
      /* **************************** */
      // By default, this gives an ID transform
      //  which means that the camera is located at (0, 0, 0)
      //  and looking at the "+z" direction
    Matrix4x4 cameraToWorld;
    double fovDegrees = 60;
    double fovRadians = Utils::degreesToRadians(fovDegrees);
    cam = new PerspectiveCamera(cameraToWorld, fovRadians, *film);



    /* ************************** */
    /* DEFINE YOUR MATERIALS HERE */
    /* ************************** */

    Material* green_100 = new Phong(Vector3D(0.2, 0.7, 0.3), Vector3D(0.2, 0.6, 0.2), 50);

    // Define and place a sphere
    Matrix4x4 sphereTransform1;
    sphereTransform1 = sphereTransform1.translate(Vector3D(-1.25, 0.5, 4.0));
    Shape* s1 = new Sphere(1.0, sphereTransform1, green_100);

    // Define and place a sphere
    Matrix4x4 sphereTransform2;
    sphereTransform2 = sphereTransform2.translate(Vector3D(1.25, 0.0, 6));
    Shape* s2 = new Sphere(1.25, sphereTransform2, green_100);

    // Define and place a sphere
    Matrix4x4 sphereTransform3;
    sphereTransform3 = sphereTransform3.translate(Vector3D(1.0, -0.75, 3.5));
    Shape* s3 = new Sphere(0.25, sphereTransform3, green_100);

    // Store the objects in the object list
    myScene.AddObject(s1);
    myScene.AddObject(s2);
    myScene.AddObject(s3);
   

    PointLightSource* myPointLight = new PointLightSource(Vector3D(0, 2.5, 3.0), Vector3D(1.0));
    myScene.AddPointLight(myPointLight); 


}



void raytrace(Camera*& cam, Shader*& shader, Film*& film,
    std::vector<Shape*>*& objectsList, std::vector<LightSource*>*& lightSourceList)
{



    Vector3D acc = (0.0, 0.0, 0.0);

    double my_PI = 0.0;
    double n_estimations = 0.0;
    unsigned int sizeBar = 40;

    size_t resX = film->getWidth();
    size_t resY = film->getHeight();

    // Main raytracing loop
    // Out-most loop invariant: we have rendered lin lines
    for (size_t lin = 0; lin < resY; lin++)
    {
        // Show progression 
        double progress = (double)lin / double(resY);
        Utils::printProgress(progress);

        // Inner loop invariant: we have rendered col columns
        for (size_t col = 0; col < resX; col++)
        {
            // Compute the pixel position in NDC
            double x = (double)(col + 0.5) / resX;
            double y = (double)(lin + 0.5) / resY;
            // Generate the camera ray
            Ray cameraRay = cam->generateRay(x, y);
            Vector3D pixelColor = Vector3D(0.0);

            // Compute ray color according to the used shader
            pixelColor += shader->computeColor(cameraRay, *objectsList, *lightSourceList);

            //TONEMAPPING SUM
            Vector3D sum = Vector3D(DELTA) + pixelColor;
            Vector3D logsum = Vector3D(log(sum.x), log(sum.y), log(sum.z));

            acc += logsum;

            // Store the pixel color
            film->setPixelValue(col, lin, pixelColor);
        }
    }

    if (REINHARD) {

        //TONEMAPPING AVG
        Vector3D avg = Vector3D(exp(acc.x / (resY * resX)), exp(acc.y / (resY * resX)), exp(acc.z / (resY * resX)));

        for (size_t lin = 0; lin < resY; lin++)
        {
            for (size_t col = 0; col < resX; col++)
            {
                Vector3D tmcolor = film->getPixelValue(col, lin) * KEY / avg;
                Vector3D burn = ((1 + tmcolor.x / (Lwhite * Lwhite)), (1 + tmcolor.y / (Lwhite * Lwhite)), (1 + tmcolor.z / (Lwhite * Lwhite)));
                tmcolor = Vector3D(tmcolor.x * burn.x / (1 + tmcolor.x), tmcolor.y * burn.y / (1 + tmcolor.y), tmcolor.z * burn.z / (1 + tmcolor.z));

                film->setPixelValue(col, lin, tmcolor);
            }
        }
    }
    else {

        // GAMMA CORRECTION
        Vector3D Vin;
        Vector3D Vout;

        for (size_t lin = 0; lin < resY; lin++)
        {
            for (size_t col = 0; col < resX; col++)
            {
                Vin = film->getPixelValue(col, lin);

                Vout = Vector3D(pow(Vin.x, GAMMA), pow(Vin.y, GAMMA), pow(Vin.z, GAMMA));

                film->setPixelValue(col, lin, Vout);
            }
        }
    }

}

//------------TASK 1---------------------//
void PaintImage(Film* film)
{
    unsigned int sizeBar = 40;

    double resX = (double)film->getWidth();
    double resY = (double)film->getHeight();

    size_t max_size = ((size_t)resY / sizeBar); 

    // Main Image Loop
    for (size_t lin = 0; lin < resY; lin++)
    {
        // Show progression
        if (lin % max_size == 0)
            std::cout << ".";

        for (size_t col = 0; col < resX; col++)
        { 
            //CHANGE...()
            Vector3D color = Vector3D(col / resX, lin / resY,0);
            //Vector3D random_color = Vector3D(1.0* col/resX, 0.0, 0.0); gradient
            //1.0 - float, 1 negre (integer)
            film->setPixelValue(col,lin, color);
           
        }
    }
}

int main()
{
    std::string separator     = "\n----------------------------------------------\n";
    std::string separatorStar = "\n**********************************************\n";
    std::cout << separator << "RT-ACG - Ray Tracer for \"Advanced Computer Graphics\"" << separator << std::endl;

    // Create an empty film
    Film *film;
    film = new Film(720, 512);


    // Declare the shader
    Vector3D bgColor(0.1, 0.1, 0.1); // Background color (for rays which do not intersect anything)
    Vector3D intersectionColor(1,0,0);
    
    //First Assignment
    Shader *shader = new IntersectionShader (intersectionColor, bgColor);
    //for each pixel of the shader a ray, red or black (when it goes to infinity)
    Shader *depthshader = new DepthShader (intersectionColor,7.5f, bgColor);
    Shader *normalshader = new NormalShader(intersectionColor, bgColor);
    Shader *whittedintegrator2 = new whittedintegrator(bgColor);
    //(... normal, whitted) ...

  

    // Build the scene---------------------------------------------------------
    // 
    // Declare pointers to all the variables which describe the scene
    Camera* cam;
    Scene myScene;
    //Create Scene Geometry and Illumiantion
    //buildSceneSphere(cam, film, myScene); //Task 2,3,4;
    buildSceneCornellBox(cam, film, myScene); //Task 5

    //---------------------------------------------------------------------------
    //Paint Image ONLY TASK 1
    PaintImage(film);

    // Launch some rays! TASK 2,3,...   
    //Material* redDiffuse = new Phong(Vector3D(0.7, 0.2, 0.3), Vector3D(0, 0, 0), 100);
    //Shader* shader_phong = new whittedintegrator(bgColor); 
    auto start = high_resolution_clock::now();
    raytrace(cam, whittedintegrator2, film, myScene.objectsList, myScene.LightSourceList);
    auto stop = high_resolution_clock::now();

    

    // Save the final result to file
    std::cout << "\n\nSaving the result to file output.bmp\n" << std::endl;
    film->save();
    film->saveEXR();

    float durationS = (durationMs(stop - start) / 1000.0).count() ;
    std::cout <<  "FINAL_TIME(s): " << durationS << std::endl;


    std::cout << "\n\n" << std::endl;
    return 0;
}
