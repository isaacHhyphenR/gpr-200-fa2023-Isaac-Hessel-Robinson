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
			cos(radians),  0, sin(radians), 0,
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


	/// <summary>
	/// Creates a right handed view space
	/// </summary>
	/// <param name="eye">: eye (camera) position</param>
	/// <param name="target">: position to look at</param>
	/// <param name="up">: up axis, usually(0,1,0)</param>
	/// <returns></returns>
	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up)
	{
		//Gram-Schmidts it up to calculate the vectors that make up the view space
		ew::Vec3 cameraForward = ew::Normalize(eye - target);
		ew::Vec3 cameraRight = ew::Normalize(ew::Cross(up, cameraForward));
		ew::Vec3 cameraUp = ew::Normalize(ew::Cross(cameraForward, cameraRight));
		//Creates inverse (transposed) rotation matrix, then translation matrixes it
		ew::Mat4 rotationMatrix = {
			cameraRight.x,	cameraRight.y,	cameraRight.z,	0,
			cameraUp.x,		cameraUp.y,		cameraUp.z,		0,
			cameraForward.x,cameraForward.y,cameraForward.z,0,
			0,				0,				0,				1
		};
		ew::Mat4 translationMatrix = {
			1,	0,	0,	-eye.x,
			0,	1,	0,	-eye.y,
			0,	0,	1,	-eye.z,
			0,	0,	0,	1
		};
		return rotationMatrix * translationMatrix;

	};
	/// <summary>
	/// Orthographic projection
	/// </summary>
	/// <param name="height"></param>
	/// <param name="aspect"></param>
	/// <param name="near"></param>
	/// <param name="far"></param>
	/// <returns></returns>
	inline ew::Mat4 Orthographic(float height, float aspect, float n, float f)
	{
		//left right bottom top
		float r = height * aspect / 2;
		float l = -r;
		float t = height / 2;
		float b = -t;


		return ew::Mat4{
			2/(r-l),	0,			0,			-((r+l) / (r-l)),
			0,			2/(t-b),	0,			-((t+b) / (t-b)),
			0,			0,			2/(f-n),	-((f+n) / (f-n)),
			0,			0,			0,			1
		};
	
	};
	/// <summary>
	/// Perspective projection
	/// </summary>
	/// <param name="field of view">: vertical aspect ratio (radians)</param>
	/// <param name="aspect"></param>
	/// <param name="near"></param>
	/// <param name="far"></param>
	/// <returns></returns>
	inline ew::Mat4 Perspective(float v, float a, float n, float f)
	{
		return ew::Mat4{
			1/(tan(v/2)*a),	0,			0,				0,
			0,				1/tan(v/2),	0,				0,
			0,				0,			(n+f)/(n-f),	(2*f*n)/(n-f),
			0,				0,			-1,				0
		};
	};



	struct Transform
	{
		ew::Vec3 position = ew::Vec3(0, 0, 0);
		ew::Vec3 rotation = ew::Vec3(0, 0, 0); //euler angles (degrees)
		ew::Vec3 scale = ew::Vec3(1, 1, 1);
		ew::Mat4 getModelMatrix() const
		{
			return Translate(position) * RotateY(rotation.y) * RotateX(rotation.x) * RotateZ(rotation.z) * Scale(scale);
		}
		explicit Transform(ew::Vec3 _position = ew::Vec3(0, 0, 0), ew::Vec3 _rotation = ew::Vec3(0, 0, 0), ew::Vec3 _scale = ew::Vec3(1, 1, 1))
		{
			position = _position;
			rotation = _rotation;
			scale = _scale;
		}
	};

}