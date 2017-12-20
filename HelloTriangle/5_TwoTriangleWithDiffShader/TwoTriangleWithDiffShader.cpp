#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// window resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.3f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

const char *secondFragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.8f, 0.8f, 1.0f);\n"
"}\n\0";

int main() {

	// init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "SecondTriangle", NULL, NULL);
	if (window == NULL) {
		std::cout << "error" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//bind window resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "error" << std::endl;
		return -1;
	}

	// crate shader and compile shader program
	// -----------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// second fragment shader
	int secondFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(secondFragmentShader, 1, &secondFragmentShaderSource, NULL);
	glCompileShader(secondFragmentShader);

	// link shader program
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// second link shader program 
	int secondShaderProgram = glCreateProgram();
	glAttachShader(secondShaderProgram, vertexShader);
	glAttachShader(secondShaderProgram, secondFragmentShader);
	glLinkProgram(secondShaderProgram);

	// clear shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(secondFragmentShader);

	// set up vertex data
	// -------------------
	float firstTriangle[] = {
		// first triangle
		-0.5f, 0.0f, 0.0f,
		-0.25f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	float secondTrangle[] = {
		// second triangle
		0.0f, 0.0f, 0.0f,
		0.25f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
	};

	unsigned int VBOs[2], VAOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondTrangle), secondTrangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// main render loop
	while (!glfwWindowShouldClose(window)) {
		// input 
		processInput(window);

		// render
		// set up bg color
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(secondShaderProgram);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// no need to unbind it every time 
		//glBindVertexArray(0);

		// event
		glfwPollEvents();

		// swap buffers
		glfwSwapBuffers(window);

	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);

	// exit
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}