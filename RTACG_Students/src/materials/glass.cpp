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

    /*
    double pi = 3.14159265358979323846; // 355/113
    bool perfect_specular = hasSpecular();
    bool gloss_diffuse = hasDiffuseOrGlossy();

    Vector3D reflected_ray_dir = n * (2 * n.dot(wi)) - wi;
    
    if (!perfect_specular || !gloss_diffuse) {
        double dot_p2 = pow(wo.dot(reflected_ray_dir), alpha);
        Vector3D specular = Ks * dot_p2; 
        Vector3D final_color = rho_d / pi + specular;
        return final_color;
    }


    return Vector3D(0);

    */

    std::cout << "Warning! Calling \"Material::getReflectance()\" for Glass. " << std::endl;

    return Vector3D(-1);

};

double Glass::getIndexOfRefraction() const
{
    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
              << std::endl;

    return -1;
}


Vector3D Glass::getEmissiveRadiance() const
{
    return Vector3D(0.0);
}


Vector3D Glass::getDiffuseReflectance() const
{
    return filter; 
}

