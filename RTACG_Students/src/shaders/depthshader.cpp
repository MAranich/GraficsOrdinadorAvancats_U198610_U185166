#include "depthshader.h"

#include "../core/utils.h"

DepthShader::DepthShader() :
    color(Vector3D(1, 0, 0))
{ }

DepthShader::DepthShader(Vector3D hitColor_, double maxDist_, Vector3D bgColor_) :
    Shader(bgColor_), maxDist(maxDist_), color(hitColor_)
{ }

Vector3D DepthShader::computeColor(const Ray &r, const std::vector<Shape*> &objList, const std::vector<LightSource*> &lsList) const
{
    //(FILL..)

    //if..

    //else...
    Intersection its;

    if (Utils::getClosestIntersection(r, objList, its)) { //its with all the intersection properties
        double distance2camera = (r.o - its.itsPoint).length(); //ray origin / intersection point
        //the bightest the nearest to the camera, 1-
        double c = std::max(0.0, 1.0 - distance2camera / maxDist);
        Vector3D color(0, c, 0);
        return color;
        //return distance2camera/7;
    }
        else {
          return bgColor;
    }

    return color;
}
