//Add algorithm for setting all vertices and indicies for each shape in procGen.h
//recommend starting with just vertex positions, and using POINTS drawing to see your vertices rendered before moving onto indices
#pragma once
#include "procGen.h"

namespace slib {
	ew::MeshData createTorus(float radius, float thickness, int numSegmentsOut, int numSegmentsIn) {
		ew::MeshData torus;

		float outStep = (2 * ew::PI) / numSegmentsOut;
		float inStep = (2 * ew::PI) / numSegmentsIn;

		for (int row = 0; row <= numSegmentsOut; row++)
		{
			float thetaOut = row * outStep;
			ew::Vec3 outPos = ew::Vec3(cos(thetaOut), sin(thetaOut), 0) * radius;
			for (int col = 0; col <= numSegmentsIn; col++)
			{
				float thetaIn = col * inStep;
				ew::Vec3 inPos = ew::Vec3(cos(thetaOut) * cos(thetaIn), cos(thetaIn) * sin(thetaOut), sin(thetaIn)) * thickness;
				ew::Vertex v;
				v.pos = inPos + outPos;
				v.normal = ew::Normalize(inPos);
				v.uv = ew::Vec2((float)col / (float)numSegmentsIn, (float)row / (float)numSegmentsOut);
				torus.vertices.push_back(v);
			}
		}

		int columns = numSegmentsIn + 1;
		for (int i = 0; i < numSegmentsOut; i++)
		{
			for (int j = 0; j < numSegmentsIn; j++)
			{
				int start = i * columns + j;

				torus.indices.push_back(start + 1);
				torus.indices.push_back(start);
				torus.indices.push_back(start + columns);

				//Triangle 2
				torus.indices.push_back(start + columns + 1);
				torus.indices.push_back(start + 1);
				torus.indices.push_back(start + columns);
			}
		}

		return torus;
	}

	ew::MeshData createSphere(float radius, int numSegments) {
		ew::MeshData sphere;

		float thetaStep = (2 * ew::PI) / numSegments;
		float phiStep = ew::PI / numSegments;
		for (int row = 0; row <= numSegments; row++)
		{
			float phi = row * phiStep;
			for (int col = 0; col <= numSegments; col++)
			{
				ew::Vertex v;
				float theta = col * thetaStep;
				v.pos.x = radius * cos(theta) * sin(phi);
				v.pos.y = radius * cos(phi);
				v.pos.z = radius * sin(theta) * sin(phi);
				v.normal = ew::Normalize(v.pos);
				v.uv = ew::Vec2((float)col / (float)numSegments, (float)row / (float)numSegments);
				sphere.vertices.push_back(v);
			}
		}
		
		//Top cap
		int poleStart = 0;
		int sideStart = numSegments + 1;
		for (int i = 0; i < numSegments; i++)
		{
			sphere.indices.push_back(sideStart + i);
			sphere.indices.push_back(poleStart + i);
			sphere.indices.push_back(sideStart + i + 1);
		}
		
		int columns = numSegments + 1;
		//Skip top and bottom poles
		for (int row = 1; row < numSegments - 1; row++)
		{
			for (int col = 0; col < numSegments; col++)
			{
				int start = row * columns + col;
				//Triangle 1
				sphere.indices.push_back(start);
				sphere.indices.push_back(start + 1);
				sphere.indices.push_back(start + columns);

				//Triangle 2
				sphere.indices.push_back(start + 1);
				sphere.indices.push_back(start + columns + 1);
				sphere.indices.push_back(start + columns);
			}
		}

		//Top cap
		poleStart = numSegments * (numSegments + 1);
		sideStart = poleStart - numSegments - 1;
		for (int i = 0; i < numSegments; i++)
		{
			sphere.indices.push_back(sideStart + i + 1);
			sphere.indices.push_back(poleStart + i);
			sphere.indices.push_back(sideStart + i);
		}

		return sphere;
	}
	
	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData cylinder;

		//Vertices
		float topY = height / 2;

		float bottomY = -topY;

		ew::Vertex top;
		top.pos.x = 0;
		top.pos.y = topY;
		top.pos.z = 0;
		top.normal = ew::Vec3(0,1.0,0);
		top.uv = ew::Vec2(0.5,0.5);
		cylinder.vertices.push_back(top);

		ew::Vertex bot;
		bot.pos.x = 0;
		bot.pos.y = bottomY;
		bot.pos.z = 0;
		bot.normal = ew::Vec3(0, -1.0, 0);
		bot.uv = ew::Vec2(0.5, 0.5);
		cylinder.vertices.push_back(bot);

		//Up top normals
		float thetaStep = (2 * ew::PI) / numSegments;
		for (int i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex v;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY;

			//Normals
			v.normal = ew::Vec3(0,1.0,0);

			//UVs
			v.uv = ew::Vec2(cos(theta) * 0.5 + 0.5, sin(theta) * 0.5 + 0.5);

			cylinder.vertices.push_back(v);
		}
		
		//Down bottom normals
		for (int i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex v;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = bottomY;

			//Normals
			v.normal = ew::Vec3(0,-1.0,0);

			//UVs
			v.uv = ew::Vec2(cos(theta) * 0.5 + 0.5, sin(theta) * 0.5 + 0.5);

			cylinder.vertices.push_back(v);
		}

		//Side top normals
		for (int i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex v2;
			v2.pos.x = cos(theta) * radius;
			v2.pos.z = sin(theta) * radius;
			v2.pos.y = topY;

			//Normals
			v2.normal = ew::Vec3(cos(theta), 0, sin(theta));

			//UVs
			v2.uv = ew::Vec2((float)i / (float)numSegments, 1);

			cylinder.vertices.push_back(v2);
		}

		//Side bottom normals
		for (int i = 0; i <= numSegments; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex v2;
			v2.pos.x = cos(theta) * radius;
			v2.pos.z = sin(theta) * radius;
			v2.pos.y = bottomY;

			//Normals
			v2.normal = ew::Vec3(cos(theta), 0, sin(theta));

			//UVs
			v2.uv = ew::Vec2((float)i / (float)numSegments, 0);

			cylinder.vertices.push_back(v2);
		}

		//Indices
		int start = 2;
		int center = 0;

		//Top indices
		for (int i = 0; i < numSegments; i++)
		{
			cylinder.indices.push_back(start + i);
			cylinder.indices.push_back(center);
			cylinder.indices.push_back(start + i + 1);
		}

		//Bottom indices
		start = 3 + numSegments;
		center = 1;
		for (int i = 0; i < numSegments; i++)
		{
			cylinder.indices.push_back(center);
			cylinder.indices.push_back(start + i);
			cylinder.indices.push_back(start + i + 1);
		}

		int sideStart = 2 + 2 * (numSegments + 1);
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
			cylinder.indices.push_back(start + columns + 1);
			cylinder.indices.push_back(start + columns);
		}

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
				plane.indices.push_back(start + columns + 1);
				plane.indices.push_back(start + columns);
			}
		}
		return plane;
	}
}