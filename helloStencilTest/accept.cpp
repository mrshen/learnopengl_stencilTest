#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "my_shader.h"
#include "stb_image.h"
#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

float cubeVertices[] = {
	// positions          // texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
float planeVertices[] = {
	// positions          // texture Coords 
	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

MyCamera cam = MyCamera(glm::vec3(.0f, .0f, 3.f), .0f, -90.f);
float camSpeed = 2.5f;
double lastXPos, lastYpos;
bool mouseFistTime = true;
double deltaTime, lastFrameTime;
float MAX_PITCH = 80.f;

void framebufferCallback(GLFWwindow*, int, int);
void mouseCallback(GLFWwindow*, double, double);
void inputProcessHandler(GLFWwindow*);

unsigned int loadTexture(char const *);

void drawTwoCubes(GLuint &cubeVAO, GLuint &cubeTex, MyShader &shader);
void drawTwoScaleCubes(GLuint &cubeVAO, MyShader &shader);
void drawPlane(GLuint &planeVAO, GLuint &planeTex, MyShader &shader);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		cout << "create window fail!" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// process func address with glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "glad fail!" << endl;
		glfwTerminate();
		return -1;
	}
	// set frame_buffer callback
	glfwSetFramebufferSizeCallback(window, framebufferCallback);

	// build and compile our shader program
	// ------------------------------------
	// vertex shader program
	MyShader shaderNormal = MyShader(
		"..\\shaders\\normal\\vertex.glsl",
		"..\\shaders\\normal\\fragment.glsl"
	);

	MyShader shaderOutline= MyShader(
		"..\\shaders\\outline\\vertex.glsl",
		"..\\shaders\\outline\\fragment.glsl"
	);
	
	// cube vbo vao
	GLuint cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// plane vbo vao
	GLuint planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// load texture
	GLuint cubeTex, planeTex;
	cubeTex = loadTexture("../res_images/marble.jpg");
	planeTex = loadTexture("../res_images/metal.png");

	shaderNormal.use();
	shaderNormal.setInt("texture1", 0);
	
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		inputProcessHandler(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 view = cam.getViewMat4();
		glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, .1f, 100.f);

		shaderNormal.use();
		shaderNormal.setMat4("view", view);
		shaderNormal.setMat4("projection", projection);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);

		// floor
		glStencilMask(0x00);
		drawPlane(planeVAO, planeTex, shaderNormal);

		// cubes
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		drawTwoCubes(cubeVAO, cubeTex, shaderNormal);
		
		// outline
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0X00);
		glDisable(GL_DEPTH_TEST);
		shaderOutline.use();
		shaderOutline.setMat4("view", view);
		shaderOutline.setMat4("projection", projection);
		drawTwoScaleCubes(cubeVAO, shaderOutline);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);

	// terminate
	glfwTerminate();
	return 0;
}

void drawTwoCubes(GLuint &cubeVAO, GLuint &cubeTex, MyShader &shader)
{
	glBindVertexArray(cubeVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cubeTex);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawTwoScaleCubes(GLuint &cubeVAO, MyShader &shader)
{
	glBindVertexArray(cubeVAO);

	float _scale = 1.05f;
	glm::vec3 scale = glm::vec3(_scale, _scale, _scale);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	model = glm::scale(model, scale);
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, scale);
	shader.setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawPlane(GLuint &planeVAO, GLuint &planeTex, MyShader &shader)
{
	glBindVertexArray(planeVAO);
	glBindTexture(GL_TEXTURE_2D, planeTex);
	shader.setMat4("model", glm::mat4(1.0f));
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (mouseFistTime == true)
	{
		lastXPos = xPos;
		lastYpos = yPos;
		mouseFistTime = false;
		//cout << xPos << "\t" << yPos << endl;
		return;
	}

	double deltaX = (xPos - lastXPos) * deltaTime; // yaw
	double deltaY = (lastYpos - yPos) * deltaTime; // pitch
	float yaw = cam.getYaw() + deltaX;
	float pitch = cam.getPitch() + deltaY;
	if (pitch > MAX_PITCH)
		pitch = MAX_PITCH;
	else if (pitch < -MAX_PITCH)
		pitch = -MAX_PITCH;

	cam.updateCamDirection(pitch, yaw);
	lastXPos = xPos;
	lastYpos = yPos;

}

void framebufferCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void inputProcessHandler(GLFWwindow* window)
{
	// close application
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}


	// handle moving camera
	float realSpeed = camSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_W, realSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_S, realSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_A, realSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_D, realSpeed);
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_Q, realSpeed);
	}
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		cam.updateCamPosition(GLFW_KEY_E, realSpeed);
	}

}

unsigned int loadTexture(char const * path)
{
	GLuint texID;
	glGenTextures(1, &texID);

	int width, height, channels;
	unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
	if (data)
	{
		GLenum format = GL_RGBA;
		if (channels == 1)
		{
			format = GL_RED;
		}
		else if (channels == 3)
		{
			format = GL_RGB;
		}

		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		cout << "load texture failed." << endl;
	}
	stbi_image_free(data);

	return texID;
}