#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../IHR/transformations.h"
namespace IHR {
	struct CameraControls {
		double prevMouseX, prevMouseY; //Mouse position from the previous frame
		float yaw = 0, pitch = 0; //degrees
		float mouseSensitivity = 0.1f; //how fast to turn with mouse
		bool firstMouse = true; //flag to store initial mouse position
		float moveSpeed = 5.0f; //how fast to move with arrow keys (M/S)
	};
	struct Camera {
		ew::Vec3 position; //Camera body position
		ew::Vec3 target; //Position to look at
		float fov; //Vertical field of view in degrees
		float aspectRatio; //Screen width / Screen height
		float nearPlane; //Near plane distance (+Z)
		float farPlane; //Far plane distance (+Z)
		bool orthographic; //Perspective or orthographic?
		float orthoSize; //Height of orthographic frustum
		ew::Mat4 ViewMatrix() //World->View
		{
			return IHR::LookAt(position, target, ew::Vec3(0, 1, 0));
		}
		
		ew::Mat4 ProjectionMatrix() //View->Clip
		{
			if (orthographic)
			{
				return IHR::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
			}
			else
			{
				return IHR::Perspective(fov, aspectRatio, nearPlane, farPlane);
			}
		}
	};
}
