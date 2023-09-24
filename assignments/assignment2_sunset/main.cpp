#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <IHR/shader.h>

struct Vertex {
	float x, y, z;
	float u, v;
};

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;


unsigned int indices[6] = {
	0, 1, 2, //triangle 1
	1, 2, 3  //triangle 2
};

float triangleColor[3] = { 1.0f, 0.5f, 0.0f };
float triangleBrightness = 1.0f;
bool showImGUIDemoWindow = true;

float _SkyColorTop[3] = { 0.9, 0.4, 0.7 };
float _SkyColorBottom[3] = { 0.6, 0.15, 0.2 };
float _SunRiseColor[3] = { 0.95,0.6,0.05 };
float _SunSetColor[3] = { 1.0,0.0,0.2 };
float _SunSpeed = 0.5;
float _HillColor[3] = { 0.1, 0.6, 0.25 };


Vertex vertices[4] = {
		//x   //y   //z   //u   //v
	 { -1.0,  1.0,  0.0,  0.0,  1.0 }, //top left
	 {  1.0,  1.0,  0.0,  1.0,  1.0 }, //top right
	 { -1.0, -1.0,  0.0,  0.0,  0.0 }, //bottom left
	 {  1.0, -1.0,  0.0,  1.0,  0.0 }  //bottom right
};

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);



int main() {
	//buncha initialization stuff
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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


/////////////////////////////////////////////////////////////////////////Actually interesting stuff

	//creates shader program
	IHR::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shader.use();

	unsigned int vao = createVAO(vertices, 4, indices, 6);
	glBindVertexArray(vao);

	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //shaded

	//render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms
		shader.setVec2("_Resolution", SCREEN_WIDTH,SCREEN_HEIGHT);
		shader.setFloat("_Time", (float)glfwGetTime());
		shader.setFloat("_SunSpeed", _SunSpeed);
		shader.setVec3("_SkyColorTop", _SkyColorTop[0], _SkyColorTop[1], _SkyColorTop[2]);
		shader.setVec3("_SkyColorBottom", _SkyColorBottom[0], _SkyColorBottom[1], _SkyColorBottom[2]);
		shader.setVec3("_SunRiseColor", _SunRiseColor[0], _SunRiseColor[1], _SunRiseColor[2]);
		shader.setVec3("_SunSetColor", _SunSetColor[0], _SunSetColor[1], _SunSetColor[2]);
		shader.setVec3("_HillColor", _HillColor[0], _HillColor[1], _HillColor[2]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Upper Sky Color", _SkyColorTop);
			ImGui::ColorEdit3("Lower Sky Color", _SkyColorBottom);
			ImGui::ColorEdit3("Risen Sun Color", _SunRiseColor);
			ImGui::ColorEdit3("Set Sun Color", _SunSetColor);
			ImGui::ColorEdit3("Foreground Color", _HillColor);
			ImGui::SliderFloat("Sun Speed", &_SunSpeed, 0.0f, 1.0f);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}



unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertexData, GL_STATIC_DRAW);

	//Define a new buffer id
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	//UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, u));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

