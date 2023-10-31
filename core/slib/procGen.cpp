//Add algorithm for setting all vertices and indicies for each shape in procGen.h
//recommend starting with just vertex positions, and using POINTS drawing to see your vertices rendered before moving onto indices
#pragma once
#include "procGen.h"

namespace slib {
	/*ew::MeshData createSphere(float radius, int numSegments) {

	}
	*/
	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData cylinder;

		//Vertices
		float topY = height / 2;
		//height * i - 0.5;
		
		float bottomY = -topY;

		cylinder.vertices.push_back({0,topY,0});
		cylinder.vertices.push_back({ 0,bottomY,0 });

		float thetaStep = (2 * ew::PI) / numSegments;
		for (int i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex v;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY;
			cylinder.vertices.push_back(v);
		}
		
		for (int i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex v;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = bottomY;
			cylinder.vertices.push_back(v);
		}

		//Indices
		int start = 2;
		int center = 0;
		
		//Top indices
		for (int i = 0; i < numSegments; i++)
		{
			cylinder.indices.push_back(start+i);
			cylinder.indices.push_back(center);
			cylinder.indices.push_back(start + i + 1);
		}
		
		//Bottom indices
		for (int i = 0; i <= numSegments; i++)
		{
			cylinder.indices.push_back(center);
			cylinder.indices.push_back(start + i);
			cylinder.indices.push_back(start + i + 1);
		}
		/*
		int sideStart = 1;
		int columns = numSegments + 1;
		for (int i = 0; i < columns; i++)
		{
			start = sideStart + i;

			//Triangle 1
			cylinder.indices.push_back(start);
			cylinder.indices.push_back(start + 1);
			cylinder.indices.push_back(start + columns);

			//Triangle 2
			cylinder.indices.push_back(start + 1);
			cylinder.indices.push_back(start + columns);
			cylinder.indices.push_back(start + columns + 1);
		}
		*/
		return cylinder;
	}
	

	ew::MeshData createPlane(float width, float height, int subdivisions) {
		ew::MeshData plane;

		for (int vertRow = 0; vertRow <= subdivisions; vertRow++) {
			for (int vertCol = 0; vertCol <= subdivisions; vertCol++) {
				ew::Vertex v;

				v.pos = ew::Vec3(width * vertCol / subdivisions, 0, -height * vertRow / subdivisions);
				v.normal = ew::Vec3(0.0, 1.0, 0.0);
				v.uv = ew::Vec2((float)vertCol / (float)subdivisions, (float)vertRow / (float)subdivisions);

				plane.vertices.push_back(v);
			}
		}

		int columns = subdivisions + 1;
		for (int indRow = 0; indRow < subdivisions; indRow++) {
			for (int indCol = 0; indCol < subdivisions; indCol++) {
				int start = indRow * columns + indCol;

				//Bottom right triangle
				plane.indices.push_back(start);
				plane.indices.push_back(start + 1);
				plane.indices.push_back(start + columns + 1);

				//Top left triangle...
				plane.indices.push_back(start);
				plane.indices.push_back(start+columns+1);
				plane.indices.push_back(start+columns);
			}
		}
		return plane;
	}
}