#include "Glass.h"

#include <iostream>




Glass::Glass(double _mu)
{
    mu = _mu; 
    filter = Vector3D(1.0); 
}

Glass::Glass(Vector3D _filter, double _mu)
{
    mu = _mu; 
    filter = _filter; 
}

Vector3D Glass::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const {

    std::cout << "Warning! Calling \"Material::getReflectance()\" for Glass. " << std::endl;

    return Vector3D(-1);

};

double Glass::getIndexOfRefraction() const
{

    return mu;
}


Vector3D Glass::getEmissiveRadiance() const
{
    return Vector3D(0.0);
}


Vector3D Glass::getDiffuseReflectance() const
{
    return filter; 
}

