#include "mirror.h"

#include <iostream>

Mirror::Mirror()
{ }

Mirror::Mirror(Vector3D Kd_, Vector3D Ks_, float alpha_):
rho_d(Kd_), Ks(Ks_), alpha(alpha_){}


Vector3D Mirror::getReflectance(const Vector3D& n, const Vector3D& wo, const Vector3D& wi) const {

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

    std::cout << "Warning! Calling \"Material::getReflectance()\" for mirror. " << std::endl;

    return Vector3D(-1);

};

double Mirror::getIndexOfRefraction() const
{
    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
              << std::endl;

    return -1;
}


Vector3D Mirror::getEmissiveRadiance() const
{
    return Vector3D(0.0);
}


Vector3D Mirror::getDiffuseReflectance() const
{
    return rho_d;
}

