#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

float vertices[21] = {
	//x		//y		//z		//r		//g		//b		//a
	-0.5,	-0.5,	0.0,	1.0,	0.0,	0.0,	1.0, //Bottom left
	0.5,	-0.5,	0.0,	0.0,	1.0,	0.0,	1.0, //Bottom right
	0.0,	0.5,	0.0,	0.0,	0.0,	1.0,	1.0  //Top center
};
const int ATTRIBS_PER_VERTEX = 7;
unsigned int createVAO(float* vertexData, int numVertices);
unsigned int createShader(GLenum shaderType, const char* sourceCode);
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);


/////////////////////// BEGINNING OF MAIN FUNCTION

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

	unsigned int vao = createVAO(vertices, 3);

	//source code for the shader program
	const char* vertexShaderSource = R"(
		#version 450
		layout(location = 0) in vec3 vPos;
		layout(location = 1) in vec4 vColor;
		out vec4 Color;
		void main(){
			Color = vColor;
			gl_Position = vec4(vPos,1.0);
		}
	)";

	const char* fragmentShaderSource = R"(
		#version 450
		out vec4 FragColor;
		in vec4 Color;
		uniform float Time = 2.0;
		void main(){
			FragColor = Color;
		}
	)";

	unsigned int shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);

	//render loop
	while (!glfwWindowShouldClose(window)) {
		float time = (float)glfwGetTime();//the current time in seconds this frame
		int timeLocation = glGetUniformLocation(shaderProgram, "_Time"); //get the location of the uniform by name
		glUniform1f(timeLocation, time);//Set the value of the variable at the location


		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

///////////////////////END OF MAIN FUNCTION


unsigned int createVAO(float* vertexData, int numVertices)
{

	unsigned int vao;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Define a new buffer ID
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//allocate space for + send vertex data to GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ATTRIBS_PER_VERTEX * numVertices, vertexData, GL_STATIC_DRAW);


	//Define position attribute (3 floats)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * ATTRIBS_PER_VERTEX, (const void*)0);
	glEnableVertexAttribArray(0);
	//define color attribute (4 floats)
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * ATTRIBS_PER_VERTEX, (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	return vao;
}


unsigned int createShader(GLenum shaderType, const char* sourceCode)
{
	//create a new vertex shader object
	unsigned int shader = glCreateShader(shaderType);
	//supply the shader object with source code
	glShaderSource(shader, 1, &sourceCode, NULL);
	//compile the shader object
	glCompileShader(shader);
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
	}
	return shader;
}


unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	//create a new vertex shader object
	unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
	//compile & link the program
	unsigned int program = glCreateProgram();
	//attach each stage
	glAttachShader(program, vertexShader);
	//glAttachShader(shaderProgram, geometryShader);
	glAttachShader(program, fragmentShader);
	//Link all the stages together
	glLinkProgram(program);
	//check for linking errors
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("Failed to link shader program: %s", infoLog);
	}
	//The linked program now contains the compiled code, so safe to delete intermediate objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return program;
}