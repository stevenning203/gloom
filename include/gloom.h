#pragma once
#include "gloomE.h"
#include <random>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>

typedef const int gloconst;
gloconst GLO_CAMERA_MODE_FREECAM = 0x20;
gloconst GLO_CAMERA_MODE_DISABLED = 0x21;
gloconst GLO_CAMERA_MODE_LIMITED = 0x22;

gloconst GLO_MOUSE_MODE_SHOW = 0x10;
gloconst GLO_MOUSE_MODE_HIDE = 0x11;

int cameraMode = GLO_CAMERA_MODE_FREECAM;

const float mouseSens = 0.2f;
const float moveSens = 5.0f;
float fovLocal = 90.0f;

int staticWidth = 0;
int staticHeight = 0;

int mouseX, mouseY;
GLFWwindow* staticWindow = NULL;
GLFWwindow* window = NULL;
float vAngle = 4.6f;
float hAngle = 5.5f;
glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camTrg = glm::vec3(0, 0, 0);
glm::vec3 camHed = glm::vec3(0, 1, 0);
glm::mat4 proj;
glm::mat4 mod = glm::mat4(1.0f);
glm::mat4 trans = glm::mat4(1.0f);
glm::mat4 rot = glm::mat4(1.0f);
glm::mat4 scale = glm::mat4(1.0f);
glm::mat4 view = glm::lookAt(camPos, camTrg, camHed);
glm::mat4 mvp;
glm::vec3 direction = glm::normalize(glm::vec3(cos(vAngle) * sin(hAngle), sin(vAngle), cos(vAngle) * cos(hAngle)));
glm::vec3 right = glm::vec3(sin(hAngle - 3.0f / 2.0f), 0, cos(hAngle - 3.0f / 2.0f));
glm::vec3 up = glm::cross(right, direction);
unsigned int MVPlocID;

auto split(std::string str, char split, bool fc = true)
{
	std::vector<float> tv;
	int current = 1;
	std::string word = "";
	for (auto x : str)
	{
		if (x == split)
		{
			if (fc)
				tv.push_back(stof(word));
			word = "";
		}
		else
		{
			word = word + x;
		}
	}
	if (fc)
		tv.push_back(stof(word));
	return tv;
}

auto split_string(std::string str, char split)
{
	std::vector<std::string> tv;
	int current = 1;
	std::string word = "";
	for (auto x : str)
	{
		if (x == split)
		{
			tv.push_back(word);
			word = "";
		}
		else
		{
			word = word + x;
		}
	}
	tv.push_back(word);
	return tv;
}

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
		if (cameraMode == GLO_CAMERA_MODE_FREECAM || cameraMode == GLO_CAMERA_MODE_LIMITED)
			view = glm::lookAt(camPos, camPos + direction, up);
		else
			view = glm::lookAt(camPos, camTrg, glm::vec3(0, 1, 0));
		return proj * view * mod * trans * rot * scale;
	}

	void setFOV(float fieldOfView)
	{
		fovLocal = fieldOfView;
		proj = glm::perspective(glm::radians(90.0f), (float)staticWidth / (float)staticHeight, 0.1f, 100.0f);
	}

	unsigned int getMVPloc()
	{
		return MVPlocID;
	}

	void setClearColor(float x, float y, float z, float a = 1.0f)
	{
		glClearColor(x, y, z, a);
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

	void setCameraTarget(glm::vec3 v)
	{
		camTrg = v;
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

	void setModTransformation(float xtrans = 0, float ytrans = 0, float ztrans = 0, float deg = 0, int x = 0, int y = 0, int z = 0, float scalex = 1.0f, float scaley = 1.0f, float scalez = 1.0f)
	{
		trans = glm::translate(glm::mat4(1.0f), glm::vec3(xtrans, ytrans, ztrans));
		rot = glm::rotate(glm::mat4(1.0f), glm::radians(deg), glm::vec3(x, y, z));
		scale = glm::scale(glm::mat4(1.0f), glm::vec3(scalex, scaley, scalez));
	}

	void resetModTransformation()
	{
		trans = glm::mat4(1.0f);
		rot = glm::mat4(1.0f);
		scale = glm::mat4(1.0f);
	}

#pragma endregion elementary

	GLFWwindow* gloInit(int windowWidth, int windowHeight, const char * windowName, unsigned int *shaderPtr, bool fullScreen = NULL)
	{
		srand((unsigned)time(0));
		int width = windowWidth;
		int height = windowHeight;
		staticWidth = width;
		staticHeight = height;
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		if (fullScreen == true)
			window = glfwCreateWindow(width, height, windowName, glfwGetPrimaryMonitor(), NULL);
		else
			window = glfwCreateWindow(width, height, windowName, NULL, NULL);
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

	void drawVAO(unsigned int VAOID, int size, glm::vec3 loc)
	{
		setModTranslation(loc[0], loc[1], loc[2]);
		glm::mat4 tmvp = getMVP();
		glBindVertexArray(VAOID);
		glUniformMatrix4fv(getMVPloc(), 1, GL_FALSE, &tmvp[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, size / sizeof(float));
	}

	void drawVAO(glm::vec3 VAOID, int size, glm::vec3 loc)
	{
		setModTranslation(loc[0], loc[1], loc[2]);
		glm::mat4 tmvp = getMVP();
		glBindVertexArray(VAOID[0]);
		glBindVertexArray(VAOID[1]);
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
		if (vAngle > glm::radians(450.0f))
		{
			vAngle = glm::radians(450.0f);
		}
		if (vAngle < glm::radians(270.0f))
		{
			vAngle = glm::radians(270.0f);
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

	unsigned int loadVertexArray(float* data, int size, float* colorData, int colorDataSize)
	{
		unsigned int a, b, c;
		glGenBuffers(1, &b);
		glBindBuffer(GL_ARRAY_BUFFER, b);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		glGenVertexArrays(1, &a);
		glBindVertexArray(a);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &c);
		glBindBuffer(GL_ARRAY_BUFFER, c);
		glBufferData(GL_ARRAY_BUFFER, colorDataSize, colorData, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		return a;
	}

	unsigned int loadVertexArray(std::vector<float> data, int size, float* colorData, int colorDataSize)
	{
		unsigned int a, b, c;
		glGenBuffers(1, &b);
		glBindBuffer(GL_ARRAY_BUFFER, b);
		glGenVertexArrays(1, &a);
		glBindVertexArray(a);
		glBufferData(GL_ARRAY_BUFFER, size, &data[0], GL_STATIC_DRAW);
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

	std::vector<float> getColorBufferMap(int numberOfVertices)
	{
		std::vector<float> colors;
		for (int i = 0; i < numberOfVertices; i++)
		{
			float currentColor = ((float)(rand() % 1000)) / 1000;
			colors.push_back(currentColor);
		}
		return colors;
	}

	unsigned int loadObjVertices(std::string pathToFile, int* nVerticesPtr, float* colorData = NULL, int colorDataSize = NULL)
	{
		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		std::vector<float> outV, outU, outN;
		std::vector<glm::vec3> vertices, normals;
		std::vector<glm::vec2> uvs;
		std::string path = pathToFile;
		std::string line;
		std::ifstream stream;
		stream.open(path);
		while (getline(stream, line))
		{
			if (line.substr(0, 2) == "v ")
			{
				line.erase(0, 2);
				std::vector<float> temp = split(line, ' ');
				vertices.push_back(glm::vec3(temp[0], temp[1], temp[2]));
			}
			if (line.substr(0, 2) == "vt")
			{
				line.erase(0, 3);
				std::vector<float> temp = split(line, ' ');
				uvs.push_back(glm::vec2(temp[0], temp[1]));
			}
			if (line.substr(0, 2) == "vn")
			{
				line.erase(0, 3);
				std::vector<float> temp = split(line, ' ');
				normals.push_back(glm::vec3(temp[0], temp[1], temp[2]));
			}
			if (line.substr(0, 2) == "f ")
			{
				line.erase(0, 2);
				std::vector<std::string> temp = split_string(line, ' ');
				std::vector<int> temp1, temp2, temp3, temp4;
				std::vector<float> it1, it2, it3, it4;
				it1 = split(temp[0], '/');
				it2 = split(temp[1], '/');
				it3 = split(temp[2], '/');
				temp1 = std::vector<int>(it1.begin(), it1.end());
				temp2 = std::vector<int>(it2.begin(), it2.end());
				temp3 = std::vector<int>(it3.begin(), it3.end());
				vertexIndices.push_back(temp1[0]);
				vertexIndices.push_back(temp2[0]);
				vertexIndices.push_back(temp3[0]);
				uvIndices.push_back(temp1[1]);
				uvIndices.push_back(temp2[1]);
				uvIndices.push_back(temp3[1]);
				normalIndices.push_back(temp1[2]);
				normalIndices.push_back(temp2[2]);
				normalIndices.push_back(temp3[2]);
			}
		}
		int n = 0;
		for (int i = 0; i < vertexIndices.size(); i++)
		{
			n++;
			unsigned int vertexIndex = vertexIndices[i];
			glm::vec3 vertex = vertices[vertexIndex - 1];
			outV.push_back((float)vertex[0]);
			outV.push_back((float)vertex[1]);
			outV.push_back((float)vertex[2]);
		}
		*nVerticesPtr = n * 3;
		for (int i = 0; i < normalIndices.size(); i++)
		{
			unsigned int normalIndex = normalIndices[i];
			glm::vec3 normal = normals[normalIndex - 1];
			outN.push_back(normal[0]);
			outN.push_back(normal[1]);
			outN.push_back(normal[2]);
		}
		for (int i = 0; i < uvIndices.size(); i++)
		{
			unsigned int uvIndex = uvIndices[i];
			glm::vec2 uv = uvs[uvIndex - 1];
			outU.push_back(uv[0]);
			outU.push_back(uv[1]);
		}
		unsigned int a, b, c;
		glGenBuffers(1, &b);
		glBindBuffer(GL_ARRAY_BUFFER, b);
		glBufferData(GL_ARRAY_BUFFER, outV.size() * sizeof(float), &outV[0], GL_STATIC_DRAW);

		glGenVertexArrays(1, &a);
		glBindVertexArray(a);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &c);
		glBindBuffer(GL_ARRAY_BUFFER, c);
		if (colorData)
			glBufferData(GL_ARRAY_BUFFER, colorDataSize, colorData, GL_STATIC_DRAW);
		else
		{
			std::vector<float> tColorBuffer = getColorBufferMap(outV.size());
			glBufferData(GL_ARRAY_BUFFER, tColorBuffer.size() * sizeof(float), &tColorBuffer[0], GL_STATIC_DRAW);
		}
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		return a;
	}
}