#include "normalshader.h"
#include "../core/utils.h"

NormalShader::NormalShader() :
    normalColor(Vector3D(1, 0, 0))
{ }


NormalShader::NormalShader(Vector3D normalColor_, Vector3D bgColor_):
    normalColor(normalColor_), Shader(bgColor_)
{}

Vector3D NormalShader::computeColor(const Ray& r, const std::vector<Shape*>& objList, const std::vector<LightSource*>& lsList) const
{
    Intersection its;

    if (Utils::getClosestIntersection(r, objList, its)) {
        return (its.normal + Vector3D(1.0, 1.0, 1.0)) / 2;
    }
    else {
        return bgColor;

    }

}