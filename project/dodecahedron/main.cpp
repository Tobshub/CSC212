//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"


const unsigned int width = 800;
const unsigned int height = 800;


// Dodecahedron vertices — positions only.
// Coordinates derived from the golden ratio phi = (1+sqrt(5))/2 ~= 1.618,
// scaled by 0.4 to fit the view.
// Groups: (+-1,+-1,+-1), (0,+-phi,+-1/phi), (+-1/phi,0,+-phi), (+-phi,+-1/phi,0)
GLfloat vertices[] =
{
	// (+-1, +-1, +-1) * 0.4
	 0.400f,  0.400f,  0.400f,  // 0
	 0.400f,  0.400f, -0.400f,  // 1
	 0.400f, -0.400f,  0.400f,  // 2
	 0.400f, -0.400f, -0.400f,  // 3
	-0.400f,  0.400f,  0.400f,  // 4
	-0.400f,  0.400f, -0.400f,  // 5
	-0.400f, -0.400f,  0.400f,  // 6
	-0.400f, -0.400f, -0.400f,  // 7
	// (0, +-phi, +-1/phi) * 0.4
	 0.000f,  0.647f,  0.247f,  // 8
	 0.000f,  0.647f, -0.247f,  // 9
	 0.000f, -0.647f,  0.247f,  // 10
	 0.000f, -0.647f, -0.247f,  // 11
	// (+-1/phi, 0, +-phi) * 0.4
	 0.247f,  0.000f,  0.647f,  // 12
	-0.247f,  0.000f,  0.647f,  // 13
	 0.247f,  0.000f, -0.647f,  // 14
	-0.247f,  0.000f, -0.647f,  // 15
	// (+-phi, +-1/phi, 0) * 0.4
	 0.647f,  0.247f,  0.000f,  // 16
	 0.647f, -0.247f,  0.000f,  // 17
	-0.647f,  0.247f,  0.000f,  // 18
	-0.647f, -0.247f,  0.000f,  // 19
};

// 12 pentagonal faces, each fan-triangulated into 3 triangles (108 indices)
GLuint indices[] =
{
	// Face  1:  0,  8,  4, 13, 12
	 0,  8,  4,    0,  4, 13,    0, 13, 12,
	// Face  2:  0, 12,  2, 17, 16
	 0, 12,  2,    0,  2, 17,    0, 17, 16,
	// Face  3:  0, 16,  1,  9,  8
	 0, 16,  1,    0,  1,  9,    0,  9,  8,
	// Face  4:  1,  9,  5, 15, 14
	 1,  9,  5,    1,  5, 15,    1, 15, 14,
	// Face  5: 16,  1, 14,  3, 17
	16,  1, 14,   16, 14,  3,   16,  3, 17,
	// Face  6:  4,  8,  9,  5, 18
	 4,  8,  9,    4,  9,  5,    4,  5, 18,
	// Face  7:  4, 13,  6, 19, 18
	 4, 13,  6,    4,  6, 19,    4, 19, 18,
	// Face  8:  2, 12, 13,  6, 10
	 2, 12, 13,    2, 13,  6,    2,  6, 10,
	// Face  9:  2, 17,  3, 11, 10
	 2, 17,  3,    2,  3, 11,    2, 11, 10,
	// Face 10:  5, 18, 19,  7, 15
	 5, 18, 19,    5, 19,  7,    5,  7, 15,
	// Face 11:  6, 19,  7, 11, 10
	 6, 19,  7,    6,  7, 11,    6, 11, 10,
	// Face 12:  7, 15, 14,  3, 11
	 7, 15, 14,    7, 14,  3,    7,  3, 11,
};

// The 30 real edges of the dodecahedron (drawn as GL_LINES, 60 indices)
GLuint edgeIndices[] =
{
	 0, 8,   0,12,   0,16,
	 1, 9,   1,14,   1,16,
	 2,10,   2,12,   2,17,
	 3,11,   3,14,   3,17,
	 4, 8,   4,13,   4,18,
	 5, 9,   5,15,   5,18,
	 6,10,   6,13,   6,19,
	 7,11,   7,15,   7,19,
	 8, 9,  10,11,  12,13,
	14,15,  16,17,  18,19,
};



int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "Spinning Dodecahedron", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);



	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");



	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));
	EBO EBO2(edgeIndices, sizeof(edgeIndices));

	// Link position attribute only
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLint colorLoc = glGetUniformLocation(shaderProgram.ID, "uColor");

	// Variables that help the rotation of the dodecahedron
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		// Simple timer
		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60)
		{
			rotation += 0.5f;
			prevTime = crntTime;
		}

		// Initializes matrices so they are not the null matrix
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		// Assigns different transformations to each matrix
		model = glm::rotate(model, glm::radians(rotation), glm::normalize(glm::vec3(1.0f, 2.0f, 0.5f)));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

		// Outputs the matrices into the Vertex Shader
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		// Pass 1: filled faces pushed back slightly so edges draw on top cleanly
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0f, 1.0f);
		glUniform3f(colorLoc, 1.0f, 0.8f, 0.2f);
		VAO1.Bind();
		EBO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		glDisable(GL_POLYGON_OFFSET_FILL);

		// Pass 2: the 30 real pentagon edges drawn as lines
		glLineWidth(2.0f);
		glUniform3f(colorLoc, 0.1f, 0.05f, 0.0f);
		EBO2.Bind();
		glDrawElements(GL_LINES, sizeof(edgeIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	EBO2.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
