#pragma once
#include "shader.h"
class whittedintegrator : public Shader {
public: 
	whittedintegrator(); 

	whittedintegrator(Vector3D bg_color); 

	virtual Vector3D computeColor(const Ray& r,
		const std::vector<Shape*>& objList,
		const std::vector<LightSource*>& lsList) const;



private:
};

