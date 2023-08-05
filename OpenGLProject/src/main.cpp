#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Basic vertex shader
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

// Basic fragment shader (calculates color output of pixels)
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n";

int main() {
	/* Initialize GlFW and GLAD
	-------------------------*/

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/* Build and compile both vertex (geometry) and fragment (pixel) shaders
	----------------------------------------------------------------------*/

	// Create vertex shader object, referenced by ID hence unsigned int datatype
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attaches shader source code vertexShaderSource to vertexShader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compiles vertex shader
	glCompileShader(vertexShader);
	// Checks for successful compile
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// Create fragment shader object, referenced by ID hence unsigned int datatype
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attaches fragment source code fragmentShaderSource to fragmentShader object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compiles fragment shader
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}
	
	/* Links the vertex and fragment shaders to our program
	-----------------------------------------------------*/

	// Create program object , referenced by ID
	unsigned int shaderProgram = glCreateProgram();
	// Attaches both vertex and fragment shaders to program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Links shaders to program
	glLinkProgram(shaderProgram);
	// Checks for successful linking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}
	// Delete shader objects once they have been linked to program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* Set up vertex and frame buffer data
	------------------------------------*/

	// Vertex input
	float vertices[] = {
		0.5f,  0.5f, 0.0f,	// top right
		0.5f, -0.5f, 0.0f,	// bottom right
	   -0.5f, -0.5f, 0.0f,	// bottom left
	   -0.5f,  0.5f, 0.0f	// top left
	};
	unsigned int indices[] = {	// note that we start from 0!
		0, 1, 3,// first triangle
		1, 2, 3 // second triangle
	};
	// Vertex Buffer Object
	unsigned int VBO, VAO, EBO;
	// TODO: Describe me
	glGenVertexArrays(1, &VAO);
	// Bind VAO
	glBindVertexArray(VAO);
	// Creates memory on GPU
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Binds buffer to GL_ARRAY_BUFFER, makes it the currently active buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copies float array of vertices into memory buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Binds buffer to GL_ARRAY_BUFFER, makes it the currently active buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Copies float array of elements into memory buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// Tells OpenGL how the data should be interpreted (Pg. 34 on LearnOpenGL)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable vertex attributes
	glEnableVertexAttribArray(0);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Draw triangles in wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);
		// Rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Activates program, every shader and rendering call after will use this program object
		glUseProgram(shaderProgram);
		// Draws primitives using the currently active shader, the previously defined 
		// vertex attribute configuration and with the VBO’s vertex data(indirectly bound via the VAO)
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}