#include "procGen.h"


namespace IHR {

	ew::MeshData createPlane(float width, float height, int subdivisions)
	{
		ew::MeshData mesh;
		int columns = subdivisions + 1;
		//Creates all the vertexes
		for (int r = 0; r <= subdivisions; r++)
		{
			for (int c = 0; c <= subdivisions; c++)
			{
				////VERTICES
				ew::Vertex vertex;
				//position
				vertex.pos.x = width * (c / subdivisions);
				vertex.pos.z = -height * (r / subdivisions);
				//UV
				vertex.uv.x = vertex.pos.x / width;
				vertex.uv.y = vertex.pos.z / -height;
				//normal
				vertex.normal = ew::Vec3(0.0f, 1.0f, 0.0f);

				mesh.vertices.push_back(vertex);


			}
		}
		//Creates all the INDICES
		for (int r = 0; r < subdivisions; r++)
		{
			for (int c = 0; c < subdivisions; c++)
			{
				//The index of the subdivisions bottom left corner
				int start = r * columns + c;
				//The bottom right triangle of this subdivision
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns + 1);
				//The top left triangle of this subdivision
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + columns + 1);
				mesh.indices.push_back(start + columns);
			}
		}
		return mesh;
	}



	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		ew::MeshData mesh;
		float topY = height / 2; //y=0 is centered
		float bottomY = -topY;
		float thetaStep = 2 * ew::PI / numSegments;

		//ring segments * 2 pairs + center * top&bottom
		float numVertices = (numSegments * 2 + 1) * 2.0f;

		////VERTICES
		////Top Center Vertex
		ew::Vertex topCenter;
		topCenter.pos.y = topY;
		topCenter.normal = ew::Vec3(0.0f, 1.0f, 0.0f);
		mesh.vertices.push_back(topCenter);

		////Top Ring Vertices
		//Adds two copies: one for vertical facing normals & one for horizontal facing
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i <= numSegments; i++)
			{
				float theta = i * thetaStep;
				bool isVertical = (j == 0);
				ew::Vertex vertex = CylinderRingVertex(theta, topCenter, radius, isVertical);
				//applies it
				mesh.vertices.push_back(vertex);
			}
		}

		////Bottom Center Vertex; calculates now but adds later
		ew::Vertex bottomCenter;
		bottomCenter.pos.y = bottomY;
		bottomCenter.normal = ew::Vec3(0.0f, -1.0f, 0.0f);

		////Bottom Ring Vertices
		//Adds two copies: one for vertical facing normals & one for horizontal facing
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i <= numSegments; i++)
			{
				float theta = i * thetaStep;
				bool isVertical = (j != 0);
				ew::Vertex vertex = CylinderRingVertex(theta, bottomCenter, radius, isVertical);
				//applies it
				mesh.vertices.push_back(vertex);
			}
		}
		//Applies the bottom center vertex
		mesh.vertices.push_back(bottomCenter);


		////INDICES
		//Top Ring Indices
		int center = 0;
		int start = center + 1;
		for (int i = 0; i < numSegments; i++)
		{
			mesh.indices.push_back(start + i);
			mesh.indices.push_back(center);
			mesh.indices.push_back(start + i + 1);
		}
		//Bottom Ring Indices
		center = mesh.vertices.size() - 1;
		start = center - numSegments - 1;
		for (int i = 0; i < numSegments; i++)
		{
			mesh.indices.push_back(start + i + 1);
			mesh.indices.push_back(center);
			mesh.indices.push_back(start + i);
		}
		//Side Indices
		int columns = numSegments + 1;
		int sideStart = numSegments + 1;
		for (int i = 0; i < columns; i++)
		{
			start = sideStart + i;
			//Triangle 1
			mesh.indices.push_back(start);
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns);
			//Triangle 2…
			mesh.indices.push_back(start + 1);
			mesh.indices.push_back(start + columns + 1);
			mesh.indices.push_back(start + columns);
		}

		return mesh;
	}

	/// <summary>
	/// Use to construct a vertex for the ring of a cylinder.
	/// </summary>
	/// <param name="theta"></param>
	/// <param name="centerVertex"></param>
	/// <param name="radius"></param>
	/// <param name="vertical"></param>
	/// <returns></returns>
	ew::Vertex CylinderRingVertex(float theta, ew::Vertex centerVertex, float radius, bool vertical)
	{
		ew::Vertex vertex;
		vertex.pos.x = cos(theta) * radius;
		vertex.pos.z = sin(theta) * radius;
		vertex.pos.y = centerVertex.pos.y;
		//adds different normals/UVs for the vertical & horizontal facing vertices
		//Vertical
		if (vertical)
		{
			vertex.normal = ew::Vec3(0.0f, -1.0f, 0.0f);
			vertex.uv.x = vertex.pos.x / ((radius - 0.5f) * 2);
			vertex.uv.y = vertex.pos.z / ((radius - 0.5f) * 2);
		}
		//Horizontal
		else
		{
			vertex.normal = ew::Normalize(vertex.pos - centerVertex.pos);
			vertex.uv.x = theta;
			vertex.uv.y = vertex.pos.y;
		}
		//returns
		return vertex;
	}



	ew::MeshData createSphere(float radius, int numSegments)
	{
		ew::MeshData mesh;
		float thetaStep = ew::TAU / numSegments;
		float phiStep = ew::PI / numSegments;
		for (int r = 0; r <= numSegments; r++)
		{
			//First and last row converge at poles
			float phi = r * phiStep;
			//Duplicate column for each row
			for (int c = 0; c <= numSegments; c++)
			{
				float theta = c * thetaStep;
				ew::Vertex vertex;
				//Position
				vertex.pos.x = radius * cos(theta) * sin(phi);
				vertex.pos.y = radius * cos(phi);
				vertex.pos.z = radius * sin(theta) * sin(phi);
				//UV
				vertex.uv.x = (float)c / numSegments;
				vertex.uv.y = (float)-r / numSegments;
				//Normal
				vertex.normal = ew::Normalize(vertex.pos - ew::Vec3(0,0,0));
				//Apply
				mesh.vertices.push_back(vertex);
			}
		}

		//// INCIDES
		///Top Pole Indices
		int poleStart = 0;
		int sideStart = numSegments + 1;
		for (int i = 0; i < numSegments; i++)
		{
			mesh.indices.push_back(sideStart + i);
			mesh.indices.push_back(poleStart + i); //Pole
			mesh.indices.push_back(sideStart + i + 1);
		}

		///Side Indices
		int columns = numSegments + 1;
		//Skip top and bottom poles
		for (int r = 1; r < numSegments - 1; r++)
		{
			for (int c = 0; c < numSegments; c++)
			{
				int start = r * columns + c;
				//Triangle 1
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns);
				//Triangle 2…
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns + 1);
				mesh.indices.push_back(start + columns);
			}
		}
		///Bottom Pole Indices
		poleStart = mesh.vertices.size() - numSegments;
		sideStart = poleStart - (numSegments + 2);
		for (int i = 0; i < numSegments; i++)
		{
			mesh.indices.push_back(sideStart + i + 1);
			mesh.indices.push_back(poleStart + i); //Pole
			mesh.indices.push_back(sideStart + i);
		}

		//Returns
		return mesh;
	}
}