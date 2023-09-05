#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[9] = {
	//x		//y		//z
	-0.5,	-0.5,	0.0, //Bottom left
	0.5,	-0.5,	0.0, //Bottom right
	0.0,	0.5,	0.0  //Top center
};

unsigned int createShader(GLenum shaderType, const char* sourceCode);

int main() {
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

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Define a new buffer ID
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//allocate space for + send vertex data to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//tell vao to pull vertex data from vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Define position attribute (3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	glEnableVertexAttribArray(0);

	//creates a new vertex array object with vertex data
	unsigned int creatVAO(float* vertexData, int numVertices);

	const char* vertexShaderSource = R"(
		#version 450
		layout(location = 0) in vec3 vPos;
		void main(){
			gl_Position = vec4(vPos,1.0);
		}
	)";

	const char* fragmentShaderSource = R"(
		#version 450
		out vec4 FragColor;
		void main(){
			FragColor = vec4(1.0);
		}
	)";

	//create a new vertex shader object
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, &vertexShaderSource);
	unsigned int fragmenthader = createShader(GL_FRAGMENT_SHADER, &fragmentShaderSource);


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createShader(GLenum shaderType, const char* sourceCode)
{
	//supply the shader object with source code
	glShaderSource(shaderType, 1, &sourceCode, NULL);
	//compile the shader object
	glCompileShader(shaderType);
	int success;
	glGetShaderiv(shaderType, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shaderType, 512, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
	}

}