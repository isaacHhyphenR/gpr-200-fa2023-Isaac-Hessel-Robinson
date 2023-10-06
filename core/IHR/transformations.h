#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"

namespace IHR
{

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
		return ew::Mat4{
		   s.x,  0,  0,  0,
			0,  s.y, 0,  0,
			0,   0, s.z, 0,
			0,   0,  0,  1
		};
	};
	//rotation around X axis (pitch)
	inline ew::Mat4 RotateX(float radians)
	{
		return ew::Mat4{
			1,  0,  0,  0,
			0,  cos(radians), -sin(radians),  0,
			0,  sin(radians), cos(radians), 0,
			0,  0,  0,  1
		};
	};
	//rotation around Y axis (yaw)
	inline ew::Mat4 RotateY(float radians)
	{
		return ew::Mat4{
			cos(radians), sin(radians),  0,  0,
			0,  1, 0,  0,
			-sin(radians), 0, cos(radians), 0,
			0,  0,  0,  1
		};
	};
	//rotation around Z axis (roll)
	inline ew::Mat4 RotateZ(float radians)
	{
		return ew::Mat4{
			cos(radians),  sin(radians), 0,  0,
			-sin(radians), cos(radians), 0,  0,
			0,  0, 1, 0,
			0,  0,  0,  1
		};
	};
	//Translate x,y,z
	inline ew::Mat4 Translate(ew::Vec3 t)
	{
		return ew::Mat4{
		    1,  0,  0, t.x,
			0,  1,	0, t.y,
			0,  0,	1, t.z,
			0,  0,  0,  1
		};
	}

	struct Transform
	{
		ew::Vec3 position = ew::Vec3(0, 0, 0);
		ew::Vec3 rotation = ew::Vec3(0, 0, 0); //euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1, 1, 1);
		ew::Mat4 getModelMatrix() const
		{
			return Translate(position) * RotateY(rotation.y) * RotateX(rotation.x) * RotateZ(rotation.z) * Scale(scale);
		}
	};

}