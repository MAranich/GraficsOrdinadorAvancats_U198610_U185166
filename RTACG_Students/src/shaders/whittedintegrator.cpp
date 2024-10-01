#include "whittedintegrator.h"

whittedintegrator::whittedintegrator()
{

}

whittedintegrator::whittedintegrator(Vector3D bg_color) : Shader(bg_color) {}


Vector3D whittedintegrator::computeColor(const Ray & r,
	const std::vector<Shape*>&objList,
	const std::vector<LightSource*>&lsList) const {



	Intersection intersection; 
	getClosestIntersection(r, objList, &intersection); 

	if intersection == NULL {
		// no direct collision with light, just ambient light 
		return bg_color;
	}

	Vector3D collision_point = intersection.itsPoint; 
	Vector3D final_color = Vector3D(0, 0, 0); 
	Vector3D wi = (intersection.itsPoint - r.o).normalized(); 

	for (int i = 0; i < lsList.lenght(); i++) {
		LightSource light = lsList[i]; 

		Vector3D pos = light.sampleLightPosition(); 
		Vector3D light_dir = pos - collision_point; 

		Ray light_ray = Ray(&collision_point, &light_dir, 0, Epsilon, 1.0); 
		bool has_collided = has_intersection(light_ray, objList); 

		if has_collided {
			continue; 
		}

		Material* mat = intersection.shape.getMaterial();
		Vector3D normal = intersection.normal; 

		Vector3D diffuse = mat::getDiffuseReflectance();
		Vector3D specular = mat::getReflectance(normal, light_dir.normalized(), wi);

		Vector3D color = diffuse + specular; 
		double coef = normal.dot(wi); 
		if coef <= 0 {
			// coef = max(coef, 0)
			coef = 0; 
		}
		color = color * coef; 

		

		final_color = final_color + color * light.getIntensity();

	}

	Vector3D ambient = Vector3D(0, 0, 0); 
	final_color = final_color + ambient;

	return ambient; 


}








