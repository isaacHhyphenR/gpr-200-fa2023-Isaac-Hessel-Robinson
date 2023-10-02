#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <IHR/texture.h>

struct Vertex {
	float x, y, z;
	float u, v;
};

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int CHARACTER_WIDTH = 128;
const int CHARACTER_HEIGHT = 128;

Vertex vertices[4] = {
	{-1.0, -1.0, 0.0, 0.0, 0.0},
	{1.0, -1.0, 0.0, 1.0, 0.0},
	{1.0, 1.0, 0.0, 1.0, 1.0},
	{-1.0, 1.0, 0.0, 0.0, 1.0}
};
unsigned short indices[6] = {
	0, 1, 2,
	2, 3, 0
};


float backgroundScale = 1;
float characterScale = 1;
float characterX = 0.5;
float characterY = 0.5;
float characterSpeed = 0.0;


int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures", NULL, NULL);
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

	//creates shaders
	ew::Shader backgroundShader("assets/background.vert", "assets/background.frag");
	ew::Shader characterShader("assets/character.vert", "assets/character.frag");

	unsigned int quadVAO = createVAO(vertices, 4, indices, 6);


	//Creates textures
	unsigned int backgroundTexture = IHR::loadTexture("assets/backgroundTree.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	unsigned int characterTexture = IHR::loadTexture("assets/dogThing.png", GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST);



	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		////BACKGROUND SHADER
		backgroundShader.use();
		//binds texture
		glBindTexture(GL_TEXTURE_2D, backgroundTexture);
		backgroundShader.setInt("_Texture", 0);
		backgroundShader.setFloat("_Scale", backgroundScale);
		//draws result
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);




		////CHARACTER SHADER
		characterShader.use();
		//binds texture
		glBindTexture(GL_TEXTURE_2D, characterTexture);
		characterShader.setInt("_Texture", 0);
		characterShader.setFloat("_Scale", characterScale);
		characterShader.setFloat("_Xpos", characterX);
		characterShader.setFloat("_Ypos", characterY);
		characterShader.setFloat("_Time", (float)glfwGetTime());
		characterShader.setFloat("_Speed", characterSpeed);
		//draws result
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);



		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();



			ImGui::Begin("Settings");
			ImGui::SliderFloat("Background Scale", &backgroundScale, 0.1f, 10.0f);
			ImGui::SliderFloat("Character Scale", &characterScale, 0.1f, 1.0f);
			ImGui::SliderFloat("Character X", &characterX, 0.0f, 1.0f);
			ImGui::SliderFloat("Character Y", &characterY, 0.0f, 1.0f);
			ImGui::SliderFloat("Character Speed", &characterSpeed, 0.0f, 1.0f);
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Vertex Buffer Object 
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*numVertices, vertexData, GL_STATIC_DRAW);

	//Element Buffer Object
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numIndices, indicesData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,x));
	glEnableVertexAttribArray(0);

	//UV attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

