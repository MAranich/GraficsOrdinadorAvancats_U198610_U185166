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


	Vector3D collision_point = intersection.itsPoint; 
	Vector3D final_color = Vector3D(0, 0, 0); 
	// w0 
	Vector3D camera_ray = (intersection.itsPoint - r.o).normalized(); 

	for (const LightSource* light : lsList) {

		Vector3D pos = light->sampleLightPosition();
		Vector3D shadow_dir = collision_point - pos;
		double shadow_length = shadow_dir.length(); 

		Ray light_ray = Ray(collision_point, shadow_dir / shadow_length, (size_t)0, Epsilon, shadow_length);
		bool has_collided = Utils::hasIntersection(light_ray, objList);

		if (has_collided) {
			continue; 
		}

		const Material& mat = intersection.shape->getMaterial();
		Vector3D normal = intersection.normal;


		Vector3D diffuse = mat.getDiffuseReflectance();
		Vector3D specular = mat.getReflectance(normal, shadow_dir.normalized(), camera_ray);

		Vector3D color = diffuse + specular;
		double coef = normal.dot(camera_ray);
		if (coef <= 0) {
			// coef = max(coef, 0)
			coef = 0;
		}


		color = color * coef;


		final_color = final_color + color * light->getIntensity();
	}

	Vector3D ambient = Vector3D(0.5, 0.0, 0.0);
	final_color = final_color + ambient;

	return final_color;


}








