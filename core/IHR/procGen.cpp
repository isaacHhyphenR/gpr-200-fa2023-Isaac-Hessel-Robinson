#include "procGen.h"


namespace IHR {
	ew::MeshData createSphere(float radius, int numSegments)
	{
		ew::MeshData mesh;
		return mesh;
	}
	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		ew::MeshData mesh;
		float topY = height / 2; //y=0 is centered
		float bottomY = -topY;
		float thetaStep = 2 * ew::PI / numSegments;

		//ring segments * 2 pairs + center * top&bottom
		float numVertices = (numSegments * 2 + 1) * 2;

		////VERTICES
		////Top Center Vertex
		ew::Vertex topCenter;
		topCenter.pos.y = topY;
		mesh.vertices.push_back(topCenter);

		////Top Ring Vertices
		//Adds two copies: one for vertical facing normals & one for horizontal facing
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i <= numSegments; i++)
			{
				ew::Vertex vertex;
				float theta = i * thetaStep;
				vertex.pos.x = cos(theta) * radius;
				vertex.pos.z = sin(theta) * radius;
				vertex.pos.y = topY;
				mesh.vertices.push_back(vertex);
			}
		}


		////Bottom Ring Vertices
		//Adds two copies: one for vertical facing normals & one for horizontal facing
		for (int j = 0; j < 2; j++)
		{
			for (int i = 0; i <= numSegments; i++)
			{
				ew::Vertex vertex;
				float theta = i * thetaStep;
				vertex.pos.x = cos(theta) * radius;
				vertex.pos.z = sin(theta) * radius;
				vertex.pos.y = bottomY;
				mesh.vertices.push_back(vertex);
			}
		}
		////Bottom Center Vertex
		ew::Vertex bottomCenter;
		bottomCenter.pos.y = bottomY;
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
		start = numVertices - (numSegments * 2);
		for (int i = 0; i < numSegments; i++)
		{
			mesh.indices.push_back(start + i);
			mesh.indices.push_back(center);
			mesh.indices.push_back(start + i + 1);
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
			mesh.indices.push_back(start + columns);
			mesh.indices.push_back(start + columns + 1);
		}

		return mesh;
	}
	ew::MeshData createPlane(float width, float height, int subdivisions)
	{
		ew::MeshData mesh;
		int columns = subdivisions + 1;
		//Creates all the vertexes & indices
		for (int r = 0; r <= subdivisions; r++)
		{
			for (int c = 0; c <= subdivisions; c++)
			{
				////VERTICES
				ew::Vertex vertex;
				//UV
				vertex.uv.x = (float)c / subdivisions;
				vertex.uv.y = (float)r / subdivisions;
				//position
				vertex.pos.x = width * (c / subdivisions);
				vertex.pos.z = -height * (r / subdivisions);
				//normal
				vertex.normal = (0.0f,0.5f,0.0f);

				mesh.vertices.push_back(vertex);

				////INDICES
				//The index of the subdivisions bottom left corner
				int start = r * columns + c;
				//The bottom right triangle of this subdivision
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + 1);
				mesh.indices.push_back(start + columns + 1);
				//The top left triangle of this subdivision
				mesh.indices.push_back(start);
				mesh.indices.push_back(start + columns);
				mesh.indices.push_back(start + columns + 1);

			}
		}
		return mesh;
	}
}