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
		return ew::Mat4(1.0f, 0.0f, 0.0f, t.x,
			0.0f, 1.0f, 0.0f, t.y,
			0.0f, 0.0f, 1.0f, t.z,
			0.0f, 0.0f, 0.0f, 1.0f);
	};

	struct Transform {
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(5.0f, 5.0f, 0.0f);  //Euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		//Construct our final model matrix
		ew::Mat4 getModelMatrix() const {
			//Must happen in this order because scale and rotate must be done around the origin (done right to left)
			return ew::Mat4(slib::Translate(position) * slib::RotateY(ew::Radians(rotation.y)) * slib::RotateX(ew::Radians(rotation.x)) * slib::RotateZ(ew::Radians(rotation.z)) * slib::Scale(scale));
		}
	};

	//Creates a right handed view space
	//eye = eye (camera) position
	//target = position to look at
	//up = up axis, usually (0,1,0)
	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 worldUp)
	{
		ew::Vec3 forward = eye - target;
		forward = ew::Normalize(forward);
		
		//use ew::Cross for cross product
		ew::Vec3 right = ew::Cross(worldUp, forward);
		right = ew::Normalize(right);

		ew::Vec3 up = Cross(forward, right);
		up = Normalize(up);

		ew::Mat4 viewMat = {
			right.x,   right.y,   right.z,   -ew::Dot(right, eye),
			up.x,      up.y,      up.z,      -ew::Dot(up, eye),
			forward.x, forward.y, forward.z, -ew::Dot(forward, eye),
			0.0,       0.0,       0.0,       1.0};

		return viewMat;
	};

	//Orthographic projection
	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far)
	{
		float width = height * aspect;
		float right = width / 2;
		float left = -(right);
		float top = height / 2;
		float bottom = -(top);
		ew::Mat4 orthoProj = {
		2 / (right - left), 0.0,                0.0,                 -((right + left) / (right - left)),
		0.0,                2 / (top - bottom), 0.0,                 -((top + bottom) / (top - bottom)),
		0.0,                0.0,                -(2 / (far - near)), -((far + near) / (far - near)),
		0.0,                0.0,                0.0,                 1.0};
		return orthoProj;
	};

	//Perspective projection
	//fov = vertical aspect ratio (radians)
	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far)
	{
		ew::Mat4 persProj = {
		1 / (tanf(ew::Radians(fov) / 2) * aspect), 0.0,                              0.0,                           0.0,
		0.0,                                       1 / (tanf(ew::Radians(fov) / 2)), 0.0,                           0.0,
		0.0,                                       0.0,                              ((near + far) / (near - far)), (2 * far * near) / (near - far),
		0.0,                                       0.0,                              -1.0,                          0.0};
		return persProj;
	};
}