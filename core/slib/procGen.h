//procGen.h
#pragma once
#include "../ew/mesh.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/ewMath.h"

namespace slib {
	ew::MeshData createTorus(float radius, float thickness, int numSegmentsOut, int numSegmentsIn);
	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float width, float height, int subdivisions);
}