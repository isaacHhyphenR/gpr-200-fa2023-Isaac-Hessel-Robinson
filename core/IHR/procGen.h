#pragma once

#include "../ew/mesh.h"


namespace IHR {
	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::Vertex CylinderRingVertex(float theta, ew::Vertex centerVertex, float radius, bool vertical);
	ew::MeshData createPlane(float width, float height, int subdivisions);
}
