#pragma once
#include "../ew/ewMath/ewMath.h"
#include "../ew/ewMath/vec4.h"
#include "../ew/ewMath/mat4.h"

namespace slib {
	struct Camera {
		ew::Vec3 position; //Camera body position
		ew::Vec3 target; //Position to look at
		float fov; //Vertical field of view in degrees
		float aspectRatio; //Screen width / Screen height
		float nearPlane; //Near plane distance (+Z)
		float farPlane; //Far plane distance (+Z)
		bool orthographic; //Perspective or orthographic?
		float orthoSize; //Height of orthographic frustrum
		ew::Mat4 ViewMatrix(); //World->View
		ew::Mat4 ProjectionMatrix(); //View->Clip
	};

	ew::Mat4 Camera::ViewMatrix()
	{
		//use LookAt()
	}

	ew::Mat4 Camera::ProjectionMatrix()
	{
		//will use either Orthographic or Perspective, depending on the orthographic bool
	}
}