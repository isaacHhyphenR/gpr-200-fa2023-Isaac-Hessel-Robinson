#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"

namespace IHR
{
	struct Transform
	{
		ew::Vec3 position = ew::Vec3(0, 0, 0);
		ew::Vec3 rotation = ew::Vec3(0, 0, 0); //euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1, 1, 1);
		ew::Mat4 getModelMatrix() const
		{

		}
	};

	//identity matrix
	inline ew::Mat4 Identity()
	{
		return ew::Mat4
		(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};
	//scale on x,y & z axes
	inline ew::Mat4 Scale(ew::Vec3 s)
	{
		
	};
	//rotation around X axis (pitch)
	inline ew::Mat4 RotateX(float radians)
	{

	};
	//rotation around Y axis (yaw)
	inline ew::Mat4 RotateY(float radians)
	{

	};
	//rotation around Z axis (roll)
	inline ew::Mat4 RotateZ(float radians)
	{

	};
	//Translate x,y,z
	inline ew::Mat4 Translate(ew::Vec3 t)
	{

	}


}