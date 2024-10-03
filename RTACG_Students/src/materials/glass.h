#ifndef GlassMATERIAL
#define GlassMATERIAL

#include "material.h"

class Glass : public Material
{
public:
    Glass(double _mu = 0.7);
    Glass(Vector3D _filter, double _mu = 0.7);

    Vector3D getReflectance(const Vector3D& n, const Vector3D& wo,
        const Vector3D& wi)const ;

    bool hasSpecular() const { return true; }
    bool hasTransmission() const { return true; }
    bool hasDiffuseOrGlossy() const { return false; }
    bool isEmissive() const { return false; }

    double getIndexOfRefraction() const;
    Vector3D getEmissiveRadiance() const;
    Vector3D getDiffuseReflectance() const;


private:
    Vector3D filter;
    double mu; 
    

};
#endif // Glass