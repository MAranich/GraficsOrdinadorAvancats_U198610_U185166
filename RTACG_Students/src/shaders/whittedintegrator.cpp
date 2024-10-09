#include "whittedintegrator.h"

whittedintegrator::whittedintegrator()
{

}

whittedintegrator::whittedintegrator(Vector3D bg_color) : Shader(bg_color) {}


Vector3D whittedintegrator::computeColor(const Ray & r,
	const std::vector<Shape*>&objList,
	const std::vector<LightSource*>&lsList) const {


	bool REMOVE_EXTRA_AMBIENT = true; 
	bool LIGHT_DECAY = true; 
	bool PARTIAL_COLOR_SHADOWS = true; 

	// ambient light
	Vector3D ambient = Vector3D(0.1);


	if (20 <= r.depth) {
		//avoid infinite recursion
		return ambient * 2.5; 
	}

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
	
	//////////////////////////////////////////////

	const Material& mat = intersection.shape->getMaterial();
	Vector3D normal = intersection.normal;

	bool material_has_specular = mat.hasSpecular();
	bool material_has_diffuse_glossy = mat.hasDiffuseOrGlossy();
	bool material_is_transparent = mat.hasTransmission();
	bool total_internal_reflection = false;
	bool from_interior = false;


	double normal_dot_wo = 0.0;
	double inner_sqrt_val = 0.0;
	double mu = 0.0;
	// ^^^ Deafult values to avoid uninitilized memory


	if (material_is_transparent) {
		// camera_ray may be -1 *
		// Vector3D inv_camera_ray = camera_ray * (-1); 
		//normal_dot_wo = camera_ray.dot(normal);

		mu = mat.getIndexOfRefraction();
		//mu = 0.7;
		from_interior = 0.0 <= camera_ray.dot(-normal); 
		if (from_interior) {  //camera_ray instead of r.d?
			// from_inside_material
			mu = 1.0 / mu;
			normal = -normal; // normal -? // Crec que sí
		}

		normal_dot_wo = camera_ray.dot(normal);

		inner_sqrt_val = 1.0 - mu * mu * (1.0 - normal_dot_wo * normal_dot_wo);

		if (inner_sqrt_val < 0.0) {
			// Total internal reflection
			total_internal_reflection = true;
			if (from_interior) {
				normal = -normal;
				// restaure normal to normal
			}
		}

	}

	if (!total_internal_reflection && material_is_transparent) {
		//material is transparent or transmissive and total internal reflection did NOT happen

		double parenthesis = mu * normal_dot_wo - sqrt(inner_sqrt_val);

		// -mu * w0
		Vector3D left_term = camera_ray * (-mu);
		// n * ( [...] )
		Vector3D right_term = normal * parenthesis;

		Vector3D refraction_dir = left_term + right_term;
		refraction_dir = refraction_dir.normalized();
		//refraction_dir.print(); 

		Ray refraction_ray = Ray(collision_point, refraction_dir, r.depth + 1);
		Vector3D refracted_color = computeColor(refraction_ray, objList, lsList);
		if (REMOVE_EXTRA_AMBIENT) {
			refracted_color = refracted_color - ambient;
		}

		// refracted_color = refracted_color * mat.getDiffuseReflectance(); 
		// TODO: ^reenable later, no se si pot ser diffuse? // Es el filtre de color que vam dir

		final_color = final_color + refracted_color;

		return final_color;
	}

	if (material_has_specular || total_internal_reflection) {
		// Is a mirror or transmissive and total internal reflection happened

		/*
			from_interior = true => mat is_transparent && total_internal_reflection
			from_interior = false =>
					material_has_specular (mirror)
					|| total_internal_reflection (from outside)
		*/

		Vector3D reflected_ray_dir = camera_ray.reflection(normal);
		Ray reflected_ray = Ray(collision_point, reflected_ray_dir, r.depth + 1);

		Vector3D reflected_color = computeColor(reflected_ray, objList, lsList);
		if (REMOVE_EXTRA_AMBIENT) {
			reflected_color = reflected_color - ambient;
		}

		final_color = final_color + reflected_color;
		return final_color; 
	}


	for (const LightSource* light : lsList) {

		Vector3D light_pos = light->sampleLightPosition(); 
		Vector3D shadow_dir = light_pos - collision_point;
		double shadow_length = shadow_dir.length(); 
		Vector3D shadow_dir_norm = shadow_dir / shadow_length;


		Ray shadow_ray = Ray(collision_point, shadow_dir_norm, (size_t)0, Epsilon, shadow_length);
		bool has_collided = Utils::hasIntersection(shadow_ray, objList);

		Vector3D color = mat.getReflectance(normal, camera_ray, shadow_dir_norm);
		double coef = normal.dot(shadow_dir_norm);
		
		if (has_collided) {
			// No direct visibility
			if (!PARTIAL_COLOR_SHADOWS) {
				continue;
			}
			// instead of just setting= to black, give partial color
			// give partial color to the shadow
			color = color * 0.2; 
			if (coef <= 0) {
				// coef = max(coef, 0)
				coef = 0;
			}
		}

		double shadow_dist_sq = 1.0; 
		if (LIGHT_DECAY) {
			shadow_dist_sq = (collision_point - light_pos).lengthSq();
			if (shadow_dist_sq < 1.0) {
				shadow_dist_sq = 1.0; 
			}
			shadow_dist_sq = sqrt(shadow_dist_sq); // patch to get decent results
			shadow_dist_sq = shadow_dist_sq * 0.2; // compensate for light strength
		}

		color = color * coef;
		Vector3D increment = color * light->getIntensity() / shadow_dist_sq;

		final_color = final_color + increment;
	}

	final_color = final_color + ambient;

	return final_color;
}








