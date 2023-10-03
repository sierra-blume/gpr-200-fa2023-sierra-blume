#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"

namespace slib {
	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f);  //Euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		//Construct our final model matrix
		ew::Mat4 getModelMatrix() const {
			//Must happen it this order because scale and rotate must be done around the origin
			Scale(scale) * RotateZ(rotation.z) * RotateX(rotation.x) * RotateY(rotation.y) * Translate(position);  //Flip this order
			return;
		}
	};

	//Identity matrix
	inline ew::Mat4 Identity() {
		return ew::IdentityMatrix();
	};

	//Scale on x,y,z axes  (scale means multiply)  (origin stays the same)
	inline ew::Mat4 Scale(ew::Vec3 s) {
		return ew::Mat4(s.x, 0.0, 0.0, 0.0,
			0.0, s.y, 0.0, 0.0,
			0.0, 0.0, s.z, 0.0,
			0.0, 0.0, 0.0, 1.0);
	};

	//Rotation around X axis (pitch) in radians
	inline ew::Mat4 RotateX(float rad) {
		//complete
	};

	//Rotation around Y axis (yaw) in radians
	inline ew::Mat4 RotateY(float rad) {
		//complete
	};

	//Rotation around Z axis (roll) in radians
	inline ew::Mat4 RotateZ(float rad) {
		//complete
	};

	//Translate x,y,z  (translate means addition)
	inline ew::Mat4 Translate(ew::Vec3 t) {
		//complete
	};
}

//in .vert
// layout(location = 0) in vec3 vPos;
// layout(location = 1) in vec3 vNormal;
// layout(location = 2) in vec2 vUV;
// //  out Surface {
// vec2 UV;
// vec3 Normal;
// }vs_out;
// 
// uniform mat4 _Model;
// 
//vs_out.UV = vUV;
//vs_out.Normal = vNormal;
//glPosition = _Model * vec4(vPos, 1.0);
//gl_Position.z *= -1.0;