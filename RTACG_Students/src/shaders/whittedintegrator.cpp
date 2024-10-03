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
		Vector3D shadow_dir = light_pos - collision_point;
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

		bool material_has_specular = mat.hasSpecular();
		bool material_has_diffuse_glossy = mat.hasDiffuseOrGlossy(); 



		bool material_is_transparent = mat.hasTransmission(); 
		bool total_internal_reflection = false; 





		double normal_dot_wo = 0.0; 
		double inner_sqrt_val = 1.0; 
		double mu = sqrt(2.0) / 2.0; 
		if (material_is_transparent) {
			if (0 <= r.d.dot(normal)) { 
				// from_inside_material
				mu = 1 / mu; 
			}

			// camera_ray may be -1 *
			normal_dot_wo = camera_ray.dot(normal); 

			inner_sqrt_val = 1 - mu * mu * (1 - normal_dot_wo * normal_dot_wo); 



			if (inner_sqrt_val < 0) {
				// Total internal reflection
				total_internal_reflection = true; 
				material_has_specular = true; 
			}


		}

		if (!total_internal_reflection) {
			//material_is_transparent

			double parenthesis = mu * normal_dot_wo - sqrt(inner_sqrt_val);

			Vector3D refraction_dir = camera_ray * (-mu) + normal * parenthesis;

			Ray refraction_ray = Ray(collision_point, refraction_dir, r.depth + 1); 
			Vector3D color = computeColor(refraction_ray, objList, lsList);

			color = color * mat.getDiffuseReflectance(); 

			final_color = final_color + color;

			continue;

		}


		if (material_has_specular) {
			// Is a mirror. 

			Vector3D reflected_ray_dir = camera_ray.reflection(normal);
			Ray reflected_ray = Ray(collision_point, reflected_ray_dir, r.depth + 1);

			Vector3D color = computeColor(reflected_ray, objList, lsList);

			final_color = final_color + color;
			continue;

		}

		Vector3D color = mat.getReflectance(normal, camera_ray, shadow_dir_norm);

		double coef = normal.dot(shadow_dir_norm);

		/* 
		if (coef == 0) printf("COEF 0\n");
		if (coef <= 0) {
			// coef = max(coef, 0)
			coef = 0;
		}
		*/


		color = color * coef;
		Vector3D increment = color * light->getIntensity(); 

		//printf("%f\n", increment.length()); 


		final_color = final_color + increment;
	}

	Vector3D ambient = Vector3D(0.05);
	final_color = final_color + ambient;

	return final_color;


}








