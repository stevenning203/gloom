#pragma once
#include "gloomE.h"
typedef const int gloconst;
gloconst GLO_CAMERA_MODE_FREECAM = 0x20;
gloconst GLO_CAMERA_MODE_DISABLED = 0x21;
gloconst GLO_CAMERA_MODE_LIMITED = 0x22;

gloconst GLO_MOUSE_MODE_SHOW = 0x10;
gloconst GLO_MOUSE_MODE_HIDE = 0x11;

int cameraMode = GLO_CAMERA_MODE_FREECAM;

const float mouseSens = 0.2f;
const float moveSens = 5.0f;

int staticWidth = 0;
int staticHeight = 0;

int mouseX, mouseY;
GLFWwindow* staticWindow = NULL;
float vAngle = 4.6f;
float hAngle = 5.5f;
glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camTrg = glm::vec3(0, 0, 0);
glm::vec3 camHed = glm::vec3(0, 1, 0);
glm::mat4 proj;
glm::mat4 mod = glm::mat4(1.0f);
glm::mat4 view = glm::lookAt(camPos, camTrg, camHed);
glm::mat4 mvp;
glm::vec3 direction = glm::normalize(glm::vec3(cos(vAngle) * sin(hAngle), sin(vAngle), cos(vAngle) * cos(hAngle)));
glm::vec3 right = glm::vec3(sin(hAngle - 3.0f / 2.0f), 0, cos(hAngle - 3.0f / 2.0f));
glm::vec3 up = glm::cross(right, direction);
unsigned int MVPlocID;

void crash()
{

}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	mouseX = (int)xpos;
	mouseY = (int)ypos;
}

namespace glo
{
#pragma region elementary

	unsigned int getKey(unsigned int glKeycode)
	{
		return glfwGetKey(staticWindow, glKeycode);
	}

	bool queueExit()
	{
		return glfwWindowShouldClose(staticWindow);
	}

	void setCameraMode(gloconst cameraModeInput)
	{
		cameraMode = cameraModeInput;
	}

	glm::vec3 getDirectionVector()
	{
		return direction;
	}

	glm::vec3 getRightVector()
	{
		return right;
	}

	glm::vec3 getUpVector()
	{
		return up;
	}

	glm::mat4 getMVP()
	{
		glm::vec3 direction = glm::normalize(glm::vec3(cos(vAngle) * sin(hAngle), sin(vAngle), cos(vAngle) * cos(hAngle)));
		glm::vec3 right = glm::vec3(sin(hAngle - 3.0f / 2.0f), 0, cos(hAngle - 3.0f / 2.0f));
		glm::vec3 up = glm::cross(right, direction);
		view = glm::lookAt(camPos, camPos + direction, up);
		return proj * view * mod;
	}

	unsigned int getMVPloc()
	{
		return MVPlocID;
	}

	void setClearColor(float x, float y, float z, float a = 1.0f)
	{
		glClearColor(x, y, z, a);
	}

	void initBuffers(float* VAOdata, int VAOsize, float* CBOdata, int CBOsize, unsigned int* ptrVAO, unsigned int* ptrCBO, unsigned int* ptrVBO = nullptr)
	{
		unsigned int VBO, CBO, VAO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBufferData(GL_ARRAY_BUFFER, VAOsize, VAOdata, GL_STATIC_DRAW);
		glGenBuffers(1, &CBO);
		glBindBuffer(GL_ARRAY_BUFFER, CBO);
		glBufferData(GL_ARRAY_BUFFER, CBOsize, CBOdata, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, CBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		*ptrVAO = VAO;
		*ptrCBO = CBO;
		if (ptrVBO != nullptr)
		{
			*ptrVBO = VBO;
		}
	}

	void enableWireframeMode()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void useShader(unsigned int shaderProgram)
	{
		glUseProgram(shaderProgram);
	}

	void flipDisplay()
	{
		glfwSwapBuffers(staticWindow);
	}

	void poll()
	{
		glfwPollEvents();
	}

	void setCameraPos(float x, float y, float z)
	{
		camPos = glm::vec3(x, y, z);
	}

	void setCameraTarget(float x, float y, float z)
	{
		camTrg = glm::vec3(x, y, z);
	}

	void setCameraHead(float x, float y, float z)
	{
		camHed = glm::vec3(x, y, z);
	}

	void getMousePos(int* ptrToX, int* ptrToY)
	{
		*ptrToX = mouseX;
		*ptrToY = mouseY;
	}

	void setMouseMode(gloconst mouseMode)
	{
		if (mouseMode == GLO_MOUSE_MODE_HIDE)
		{
			glfwSetInputMode(staticWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else if (mouseMode == GLO_MOUSE_MODE_SHOW)
		{
			glfwSetInputMode(staticWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{

		}
	}

	void setMousePos(int x, int y)
	{
		glfwSetCursorPos(staticWindow, x, y);
	}

	void terminate()
	{
		glfwTerminate();
	}

	void forceLowInput()
	{
		glFinish();
	}

	void clearBuffer()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	double getTime()
	{
		return glfwGetTime();
	}

	void setModTranslation(float x, float y, float z)
	{
		mod = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
	}

#pragma endregion elementary

	GLFWwindow* gloInit(int windowWidth, int windowHeight, const char * windowName, unsigned int *shaderPtr)
	{
		int width = windowWidth;
		int height = windowHeight;
		staticWidth = width;
		staticHeight = height;
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		GLFWwindow* window = glfwCreateWindow(width, height, windowName, glfwGetPrimaryMonitor(), NULL);
		proj = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f);
		staticWindow = window;
		glfwMakeContextCurrent(window);
		if (!window)
		{
			std::cout << "Failed to init GLFW window" << std::endl;
			glfwTerminate();
			return NULL;
		}
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return NULL;
		}
		glViewport(0, 0, width, height);
		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
		unsigned int shaderProgram = glE::shaderInit("res/shaders/basic.shader");
		MVPlocID = glGetUniformLocation(shaderProgram, "MVP");
		*shaderPtr = shaderProgram;
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glfwSetCursorPosCallback(staticWindow, cursorPosCallback);
		return window;
	}

	void drawVAO(unsigned int VAOID, int size, float x, float y, float z)
	{
		setModTranslation(x, y, z);
		glm::mat4 tmvp = getMVP();
		glBindVertexArray(VAOID);
		glUniformMatrix4fv(getMVPloc(), 1, GL_FALSE, &tmvp[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, size / sizeof(float));
	}

	void drawVAO(unsigned int VAOID, int size, glm::vec3 loc)
	{
		setModTranslation(loc[0], loc[1], loc[2]);
		glm::mat4 tmvp = getMVP();
		glBindVertexArray(VAOID);
		glUniformMatrix4fv(getMVPloc(), 1, GL_FALSE, &tmvp[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, size / sizeof(float));
	}

	void cameraUpdate(float deltaTime)
	{
		if (cameraMode == GLO_CAMERA_MODE_DISABLED)
		{
			return;
		}
		int mouseX = 0, mouseY = 0, deltaMX = 0, deltaMY = 0;
		setMousePos(staticWidth / 2, staticHeight / 2);
		getMousePos(&mouseX, &mouseY);
		deltaMX = staticWidth / 2 - (int)mouseX;
		deltaMY = staticHeight / 2 - (int)mouseY;
		hAngle += mouseSens * deltaTime * deltaMX;
		vAngle += mouseSens * deltaTime * deltaMY;
		if (vAngle > 8)
		{
			vAngle = 8;
		}
		if (vAngle < 4.5)
		{
			vAngle = 4.5;
		}
		glm::vec3 direction = glm::normalize(glm::vec3(cos(vAngle) * sin(hAngle), sin(vAngle), cos(vAngle) * cos(hAngle)));
		glm::vec3 right = glm::vec3(sin(hAngle - 3.0f / 2.0f), 0, cos(hAngle - 3.0f / 2.0f));
		glm::vec3 up = glm::cross(right, direction);

		if (cameraMode == GLO_CAMERA_MODE_FREECAM)
		{
			if (glfwGetKey(staticWindow, GLFW_KEY_W) == GLFW_PRESS)
			{
				camPos += direction * deltaTime * moveSens;
			}
			if (glfwGetKey(staticWindow, GLFW_KEY_S) == GLFW_PRESS)
			{
				camPos -= direction * deltaTime * moveSens;
			}
			if (glfwGetKey(staticWindow, GLFW_KEY_D) == GLFW_PRESS)
			{
				camPos += right * deltaTime * moveSens;
			}
			if (glfwGetKey(staticWindow, GLFW_KEY_A) == GLFW_PRESS)
			{
				camPos -= right * deltaTime * moveSens;
			}
			if (glfwGetKey(staticWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				camPos[1] += deltaTime * moveSens;
			}
			if (glfwGetKey(staticWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			{
				camPos[1] -= deltaTime * moveSens;
			}
		}
	}

	unsigned int loadVertexArray(float *data, int size, float *colorData, int colorDataSize)
	{
		unsigned int a, b, c;
		glGenBuffers(1, &b);
		glBindBuffer(GL_ARRAY_BUFFER, b);
		glGenVertexArrays(1, &a);
		glBindVertexArray(a);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glGenBuffers(1, &c);
		glBindBuffer(GL_ARRAY_BUFFER, c);
		glBufferData(GL_ARRAY_BUFFER, colorDataSize, colorData, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, b);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, c);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		return a;
	}
}