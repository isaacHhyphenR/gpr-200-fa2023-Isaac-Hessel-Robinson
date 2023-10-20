#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <IHR/shader.h>
#include <ew/procGen.h>
#include <IHR/transformations.h>
#include <IHR/camera.h>
#include <IHR/IHRmath.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
IHR::Transform cubeTransforms[NUM_CUBES];

IHR::Camera mainCamera;
IHR::CameraControls mainCameraControls;

void MoveCamera(GLFWwindow* window, IHR::Camera* camera, IHR::CameraControls* controls);

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	IHR::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5f;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5f;
	}

	///CAMERA
	mainCamera.position = (0.0f, 0.0f, 0.0f);
	mainCamera.position.z = 5.0f; //if I set the position overal it sets them all to the same as Z
	mainCamera.target = (0.0f, 0.0f, 0.0f);
	mainCamera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	mainCamera.fov = 60;
	mainCamera.orthoSize = 6;
	mainCamera.nearPlane = 0.1f;
	mainCamera.farPlane = 100.0f;

	

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		MoveCamera(window, &mainCamera, &mainCameraControls);

		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();

		//convert to view & clipspace
		shader.setMat4("_ViewProjection", mainCamera.ProjectionMatrix() * mainCamera.ViewMatrix());
		//TODO: Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", cubeTransforms[i].getModelMatrix());
			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Text("Cubes");
			for (size_t i = 0; i < NUM_CUBES; i++)
			{
				ImGui::PushID((int)i);
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::DragFloat3("Position", &cubeTransforms[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &cubeTransforms[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &cubeTransforms[i].scale.x, 0.05f);
				}
				ImGui::PopID();
			}
			ImGui::Text("Camera");
			ImGui::Checkbox("Orthographic", &mainCamera.orthographic);
			ImGui::DragFloat3("Position", &mainCamera.position.x, 0.05f);
			ImGui::DragFloat3("Target", &mainCamera.target .x, 0.05f);
			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void MoveCamera(GLFWwindow* window, IHR::Camera* camera, IHR::CameraControls* controls)
{
	//If right mouse is not held, release cursor and return early.
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
		//Release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls->firstMouse = true;
		return;
	}
	//GLFW_CURSOR_DISABLED hides the cursor, but the position will still be changed as we move our mouse.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Get screen mouse position this frame
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	//If we just started right clicking, set prevMouse values to current position.
	//This prevents a bug where the camera moves as soon as we click.
	if (controls->firstMouse) {
		controls->firstMouse = false;
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}

	//Add mosue delta to yaw & pitch
	controls->yaw += (float)(mouseX - controls->prevMouseX) * controls->mouseSensitivity;
	controls->pitch -= (float)(mouseY - controls->prevMouseY) * controls->mouseSensitivity;
	controls->pitch = IHR::clamp(controls->pitch, -89, 89);

	//Remember previous mouse position
	controls->prevMouseX = mouseX;
	controls->prevMouseY = mouseY;

	//Construct forward vector using yaw and pitch.
	float fYaw = ew::Radians(controls->yaw);
	float fPitch = ew::Radians(controls->pitch);
	ew::Vec3 forward = ew::Vec3(cos(fYaw) * cos(fPitch), sin(fPitch), sin(fYaw) * cos(fPitch));
	//Sets the target to a position in front of the camera
	camera->target = camera->position + forward;

}

