#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/ewMath.h"

namespace slib {
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
		ew::Mat4 m = Identity();
		m[1][1] = cos(rad);
		m[2][1] = -sin(rad);
		m[1][2] = sin(rad);
		m[2][2] = cos(rad);
		return m;
	};

	//Rotation around Y axis (yaw) in radians
	inline ew::Mat4 RotateY(float rad) {
		ew::Mat4 m = Identity();
		m[0][0] = cos(rad);
		m[2][0] = sin(rad);
		m[0][2] = -sin(rad);
		m[2][2] = cos(rad);
		return m;
	};

	//Rotation around Z axis (roll) in radians
	inline ew::Mat4 RotateZ(float rad) {
		ew::Mat4 m = Identity();
		m[0][0] = cos(rad);
		m[1][0] = -sin(rad);
		m[0][1] = sin(rad);
		m[1][1] = cos(rad);
		return m;
	};

	//Translate x,y,z  (translate means addition)
	inline ew::Mat4 Translate(ew::Vec3 t) {
		return ew::Mat4(1.0, 0.0, 0.0, t.x,
			0.0, 1.0, 0.0, t.y,
			0.0, 0.0, 1.0, t.z,
			0.0, 0.0, 0.0, 1.0);
	};

	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f);  //Euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		//Construct our final model matrix
		ew::Mat4 getModelMatrix() const {
			//Must happen in this order because scale and rotate must be done around the origin (done right to left)
			return Translate(position) * RotateY(ew::Radians(rotation.y)) * RotateX(ew::Radians(rotation.x)) * RotateZ(ew::Radians(rotation.z)) * Scale(scale);
		}
	};
}