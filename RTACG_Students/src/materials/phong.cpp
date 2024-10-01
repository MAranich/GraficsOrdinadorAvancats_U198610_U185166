#include "phong.h"

#include <iostream>

Phong::Phong()
{ }

Phong::Phong(Vector3D Kd_, Vector3D Ks_, float alpha_):
rho_d(Kd_), Ks(Ks_), alpha(alpha_){}


Vector3D Phong::getReflectance(const Vector3D& n, const Vector3D& wo,
    const Vector3D& wi) const {

    //FILL(...)
    double dot_p = (n * wi).length();
    Vector3D wr = n * 2 * dot_p - wi;
    double dot_p2 = pow((wo * wr).length(), alpha);
    Vector3D specular = Ks * dot_p2;
    double pi = 3.14159265358979323846;
    Vector3D fs = rho_d / pi + specular;
    return fs;

};

double Phong::getIndexOfRefraction() const
{
    std::cout << "Warning! Calling \"Material::getIndexOfRefraction()\" for a non-transmissive material"
              << std::endl;

    return -1;
}


Vector3D Phong::getEmissiveRadiance() const
{
    return Vector3D(0.0);
}


Vector3D Phong::getDiffuseReflectance() const
{
    return rho_d;
}

