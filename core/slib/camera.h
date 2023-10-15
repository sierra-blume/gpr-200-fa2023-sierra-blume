#pragma once
#include "../ew/ewMath/ewMath.h"
#include "../ew/external/glad.h"
#include "transformations.h"
#include <imgui_impl_glfw.h>

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
		LookAt(position, target, ew::Vec3(0, 1, 0));
	}

	ew::Mat4 Camera::ProjectionMatrix()
	{
		//either Orthographic or Perspective depending on the orthographic bool
		if (orthographic == true)
		{
			return Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
		}
		else
		{
			return Perspective(fov, aspectRatio, nearPlane, farPlane);
		}
	}

	struct CameraControls {
		double prevMouseX, prevMouseY; //Mouse position from previous frame
		float yaw = 0, pitch = 0; //Degrees
		float mouseSensitivity = 0.1f; //How fast to turn with mouse
		bool firstMouse = true; //Flag to store initial mouse position
		float moveSpeed = 5.0f; //How fast to move with arrow keys (M/S)
		void moveCamera(GLFWwindow* window, Camera* camera, CameraControls controls);
	};

	void CameraControls::moveCamera(GLFWwindow* window, Camera* camera, CameraControls controls)
	{
		//figure out later//
	}
}