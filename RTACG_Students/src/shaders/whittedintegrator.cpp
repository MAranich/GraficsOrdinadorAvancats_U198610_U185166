#include "whittedintegrator.h"

whittedintegrator::whittedintegrator()
{

}

whittedintegrator::whittedintegrator(Vector3D bg_color) : Shader(bg_color) {}


Vector3D whittedintegrator::computeColor(const Ray & r,
	const std::vector<Shape*>&objList,
	const std::vector<LightSource*>&lsList) const {


	Intersection intersection; 
	bool intersection_exists = Utils::getClosestIntersection(r, objList, intersection); 

	if (!intersection_exists) {
		// no direct collision with light, just ambient light 
		return Shader::bgColor;
	}

	//we have a collision
	Vector3D collision_point = intersection.itsPoint; 
	// accumulator for the final result
	Vector3D final_color = Vector3D(0, 0, 0); 
	// w0 (in reality = -camera ray)
	Vector3D camera_ray = (r.o - intersection.itsPoint).normalized();
	

	//printf("%d", lsList.size()); 
	for (const LightSource* light : lsList) {

		Vector3D light_pos = light->sampleLightPosition(); 
		Vector3D shadow_dir = collision_point - light_pos;
		double shadow_length = shadow_dir.length(); 
		Vector3D shadow_dir_norm = shadow_dir / shadow_length;

		Ray shadow_ray = Ray(collision_point, shadow_dir_norm, (size_t)0, Epsilon, shadow_length);
		bool has_collided = Utils::hasIntersection(shadow_ray, objList);

		if (has_collided) {
			continue; 
		}

		const Material& mat = intersection.shape->getMaterial();
		Vector3D normal = intersection.normal;
		normal = normal.normalized(); 


		Vector3D diffuse = mat.getDiffuseReflectance();
		Vector3D specular = mat.getReflectance(normal, camera_ray, shadow_dir_norm);

		Vector3D color = diffuse + specular;
		double coef = normal.dot(shadow_dir_norm);

		if (coef == 0) printf("COEF 0\n");
		if (coef <= 0) {
			// coef = max(coef, 0)
			coef = 0;
		}



		color = color * coef;
		Vector3D increment = color * light->getIntensity(); 

		//printf("%f\n", increment.length()); 


		final_color = final_color + increment;
	}

	Vector3D ambient = Vector3D(0.1, 0.0, 0.0);
	final_color = final_color + ambient;

	return final_color;


}








