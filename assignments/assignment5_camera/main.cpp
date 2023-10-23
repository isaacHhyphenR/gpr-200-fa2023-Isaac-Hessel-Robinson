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

void MoveCamera(GLFWwindow* window, IHR::Camera* camera, IHR::CameraControls* controls, float deltaTime);

const ew::Vec3 CAM_DEFAULT_POS = (0.0f, 0.0f, 0.0f);
const float CAM_DEFAULT_Z = 2.0f; //if I set the position overal it sets them all to the same as Z
const ew::Vec3 CAM_DEFAULT_TARGET = (0.0f, 0.0f, 0.0f);
const float CAM_DEFAULT_FOV = 125;
const float CAM_DEFAULT_ORTHO_SIZE = 6;
const float CAM_DEFAULT_NEAR = 0.1f;
const float CAM_DEFAULT_FAR = 100.0f;

const float CAM_DEFAULT_SPEED = 2.0f;
const float CAM_DEFAULT_YAW = -90;
const float CAM_DEFAULT_PITCH = 0;

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
	mainCamera.position = CAM_DEFAULT_POS;
	mainCamera.position.z = CAM_DEFAULT_Z; //if I set the position overal it sets them all to the same as Z
	mainCamera.target = CAM_DEFAULT_TARGET;
	mainCamera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	mainCamera.fov = CAM_DEFAULT_FOV;
	mainCamera.orthoSize = CAM_DEFAULT_ORTHO_SIZE;
	mainCamera.nearPlane = CAM_DEFAULT_NEAR;
	mainCamera.farPlane = CAM_DEFAULT_FAR;

	mainCameraControls.moveSpeed = CAM_DEFAULT_SPEED;
	mainCameraControls.yaw = CAM_DEFAULT_YAW;
	mainCameraControls.pitch = CAM_DEFAULT_PITCH;
	

	float prevTime = (float)glfwGetTime(); //Timestamp of previous frame
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Calculate deltaTime
		float time = (float)glfwGetTime(); //Timestamp of current frame
		float deltaTime = time - prevTime;
		prevTime = time;

		//Pass deltaTime into moveCamera. Update this function to include a 4th parameter.
		MoveCamera(window, &mainCamera, &mainCameraControls, deltaTime);


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
			ImGui::DragFloat3("Target", &mainCamera.target.x, 0.05f);
			ImGui::DragFloat("Field of View", &mainCamera.fov);
			ImGui::DragFloat("Near Plane", &mainCamera.nearPlane);
			ImGui::DragFloat("Far Plane", &mainCamera.farPlane);
			ImGui::Text("Camera Controls");
			ImGui::Text("Yaw:   %i", (int)mainCameraControls.yaw);
			ImGui::Text("Pitch: %i", (int)mainCameraControls.pitch);
			ImGui::DragFloat("Move Speed", &mainCameraControls.moveSpeed);
			if (ImGui::Button("Reset")) {
				mainCamera.position = CAM_DEFAULT_POS;
				mainCamera.position.z = CAM_DEFAULT_Z; //if I set the position overal it sets them all to the same as Z
				mainCamera.target = CAM_DEFAULT_TARGET;
				mainCamera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
				mainCamera.fov = CAM_DEFAULT_FOV;
				mainCamera.orthoSize = CAM_DEFAULT_ORTHO_SIZE;
				mainCamera.nearPlane = CAM_DEFAULT_NEAR;
				mainCamera.farPlane = CAM_DEFAULT_FAR;
				mainCameraControls.yaw = CAM_DEFAULT_YAW;
				mainCameraControls.pitch = CAM_DEFAULT_PITCH;
			}
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


void MoveCamera(GLFWwindow* window, IHR::Camera* camera, IHR::CameraControls* controls, float deltaTime)
{
	//If right mouse is not held, release cursor
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
	{
		//Release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls->firstMouse = true;
	}
	//Only do rotation stuff when you hold down the mouse
	else
	{
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

		//Add mouse delta to yaw & pitch
		controls->yaw += (float)(mouseX - controls->prevMouseX) * controls->mouseSensitivity;
		controls->pitch -= (float)(mouseY - controls->prevMouseY) * controls->mouseSensitivity;
		controls->pitch = IHR::clamp(controls->pitch, -89, 89);

		//Remember previous mouse position
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}

	///////////////Construct forward vector using yaw and pitch.
	float RYaw = ew::Radians(controls->yaw);
	float RPitch = ew::Radians(controls->pitch);
	ew::Vec3 forward = ew::Vec3(cos(RYaw) * cos(RPitch), sin(RPitch), sin(RYaw) * cos(RPitch));

	///////////////MOVEMENT
	//Gram-Schmidts it up to calculate the vectors that make up the view space
	ew::Vec3 right = ew::Normalize(ew::Cross(ew::Vec3(0, 1, 0), forward));
	ew::Vec3 up = ew::Normalize(ew::Cross(forward, right));
	//Keyboard controls for moving along forward, back, right, left, up, and down.
	if (glfwGetKey(window, GLFW_KEY_W)) {
		camera->position += forward * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		camera->position -= forward * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		camera->position += right * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		camera->position -= right * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_E)) {
		camera->position += up * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_Q)) {
		camera->position -= up * controls->moveSpeed * deltaTime;
	}


	///////////////Sets the target to a position in front of the camera
	camera->target = camera->position + forward;
}

