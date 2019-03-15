//
//  main.cpp
//  OpenGL Shadows
//
//  Created by CGIS on 05/12/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#define GLEW_STATIC



#include <iostream>
#include "glm/glm.hpp"//core glm functionality
#include "glm/gtc/matrix_transform.hpp"//glm extension for generating common transformation matrices
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include <string>
#include "Shader.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"
#define TINYOBJLOADER_IMPLEMENTATION

#include "Model3D.hpp"
#include "Mesh.hpp"

int glWindowWidth = 1640;
int glWindowHeight = 880;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

GLfloat pitch = 0.0f;
GLfloat yaw = -90.0f;
GLfloat pitchIncrement = 1.6f;
GLfloat yawIncrement = 1.6f;
GLdouble lastX = glWindowWidth / 2;
GLdouble lastY = glWindowHeight / 2;

const GLuint SHADOW_WIDTH = 6000, SHADOW_HEIGHT = 6000;

glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat3 lightDirMatrix;
GLuint lightDirMatrixLoc;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightPosLamp1;
GLuint lightPosLocLamp1;
glm::vec3 lightPosLamp2;
GLuint lightPosLocLamp2;
glm::vec3 lightPosLamp3;
GLuint lightPosLocLamp3;
glm::vec3 lightPosLamp4;
GLuint lightPosLocLamp4;
glm::vec3 lightColor;
GLuint lightColorLoc;
glm::vec3 lightColor2;
GLuint lightColorLoc2;

glm::vec3 lightSpotLamp1;
GLuint lightSpotLocLamp1;
glm::vec3 lightSpotLamp2;
GLuint lightSpotLocLamp2;
glm::vec3 lightSpotLamp3;
GLuint lightSpotLocLamp3;
glm::vec3 lightSpotLamp4;
GLuint lightSpotLocLamp4;


gps::Camera myCamera(glm::vec3(0.0f, 1.0f, 2.5f), glm::vec3(0.0f, 0.0f, 0.0f));
GLfloat cameraSpeed = 1.01f;
GLfloat scrollCameraSpeed = 0.1f;

bool pressedKeys[1024];
GLfloat angle;
GLfloat lightAngle;

gps::Model3D myModel;
gps::Model3D ground;
gps::Model3D town;
gps::Model3D tree;
gps::Model3D fountain;
gps::Model3D bench;
gps::Model3D stone;
gps::Model3D lamp;
gps::Model3D lightCube;
gps::Model3D boy;
gps::Model3D boy2;
gps::Model3D ball;
gps::Model3D dog;
gps::Model3D car;
gps::Model3D water;
gps::Model3D drop;
gps::Model3D carRight;
gps::Model3D carLeft;

gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader depthMapShader;

GLuint shadowMapFBO;
GLuint depthMapTexture;

std::vector<const GLchar*> faces;
gps::SkyBox mySkyBox;
gps::Shader skyBoxShader;

GLfloat computeFog = 0.0f;
GLuint computeFogLoc;

GLfloat deplasareX = -1.0;
GLfloat deplasareZ = -0.1;
GLfloat indiceZ = 0.01;
GLfloat indiceX = 0.02;
GLfloat indiceRotation = 5.0f;
GLfloat ballRotation = 1.0f;

GLfloat carRotation = 0.0f;
GLfloat carXYZ = 4.7f;
//GLfloat carSpeed = 0.1f;
GLfloat carSpeed = 0.2f;
glm::vec3 carFront;
glm::vec3 carBack;
glm::vec3 carLocation;
GLboolean followCar = GL_TRUE;
GLboolean cade = false;
GLfloat indiceCadere = 5.0f;
GLfloat indiceCarRotation = 0.0f;


const int DropNo = 200;
typedef struct
{
	float Xpos;
	float Ypos;
	float Zpos;
	float Xmov;
	float Zmov;
	float Direction;
	float Acceleration;
	float Deceleration;
	float Scalez;
	bool Visible;
}Drops;
Drops Drop[DropNo];

int noOfDroplets = 1000;
float dropletsX[42000];
float dropletsY[42000];
float dropletsZ[42000];
float dropScale = 0.01f;
GLboolean rain = false;
int sus = 1;

GLboolean night = false;


volatile float watermovement = 0.5, waterFlag = 0, waterhight = -3.0, waterZ = 0.5;

double randomFunction(void)
{
	return ((5 * rand() % 11 + 5)*rand() % 11 + 1)*0.008;
}
double randomFunctionMove(void)
{
	return ((5 * rand() % 11 + 5)*rand() % 11 + 1)*0.0013;
}
void glCreateDrops(void)
{
	int i;


	for (i = 0; i < DropNo; i++)
	{
		Drop[i].Xpos = 2.0f;
		Drop[i].Ypos = 0;
		Drop[i].Zpos = 2.4f;
		Drop[i].Xmov = randomFunctionMove() - randomFunctionMove();
		Drop[i].Zmov = randomFunctionMove() - randomFunctionMove();
		Drop[i].Scalez = 0.01;
		Drop[i].Direction = 0;
		Drop[i].Acceleration = randomFunction();
		Drop[i].Deceleration = 0.0025;
	}
}

void glUpdateDrops(void)
{
	int i;
	for (i = 0; i < DropNo; i++)
	{
		Drop[i].Ypos = Drop[i].Ypos + Drop[i].Acceleration - Drop[i].Deceleration;
		Drop[i].Deceleration = Drop[i].Deceleration + 0.0025;

		Drop[i].Xpos = Drop[i].Xpos + Drop[i].Xmov;
		Drop[i].Zpos = Drop[i].Zpos + Drop[i].Zmov;
		Drop[i].Direction = Drop[i].Direction + (((int)(rand() % 30 * 0.5) + 1)  * rand() % 30) + 1;
		if (Drop[i].Ypos < -5)
		{
			Drop[i].Xpos = 2.0f;
			Drop[i].Ypos = 0.0f;
			Drop[i].Zpos = 2.4f;
			Drop[i].Direction = 100;
			Drop[i].Acceleration = randomFunction();
			Drop[i].Deceleration = 0.0025;
		}
	}
}

void glDrawDrops(void)
{
	for (int i = 0; i < DropNo; i++)
	{

		//create model matrix for fountain
		model = glm::translate(glm::mat4(1.0f), glm::vec3(Drop[i].Xpos, Drop[i].Ypos, Drop[i].Zpos));

		model = glm::rotate(model, glm::radians(Drop[i].Direction - 90.0f), glm::vec3(0, 0, 1));
		model = glm::scale(model, glm::vec3(Drop[i].Scalez, Drop[i].Scalez, Drop[i].Scalez));
		//send model matrix data to shader	
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//compute normal matrix
		normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
		//send normal matrix data to shader
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		drop.Draw(myCustomShader);

	}
}
float getRand(float a, float b)
{
	int randomNumber = rand() % (int((b - a) * 1000));
	float no = (float)randomNumber / 1000.0;
	return (no + a);
}


GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO
	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	myCustomShader.useShaderProgram();

	//set projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	//send matrix data to shader
	GLint projLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	
	lightShader.useShaderProgram();
	
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	//set Viewport transform
	glViewport(0, 0, retina_width, retina_height);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
	{
		if (xpos < lastX)
		{
			yaw += yawIncrement;
			if (yaw > 360.0f)
				yaw -= 360.0f;
			myCamera.rotate(pitch, yaw);
		}
		else if (xpos > lastX)
		{
			yaw -= yawIncrement;
			if (yaw < 0.0f)
				yaw += 360.0f;
			myCamera.rotate(pitch, yaw);
		}

		if (ypos < lastY)
		{
			pitch -= pitchIncrement;
			if (pitch < 0.0f)
				pitch += 360.0f;
			myCamera.rotate(pitch, yaw);
		}
		else if (ypos > lastY)
		{
			pitch += pitchIncrement;
			if (pitch > 360.0f)
				pitch -= 360.0f;
			myCamera.rotate(pitch, yaw);
		}

		lastX = xpos;
		lastY = ypos;
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0)
	{
		myCamera.move(gps::MOVE_FORWARD, scrollCameraSpeed);
	}
	else if (yoffset < 0)
	{
		myCamera.move(gps::MOVE_BACKWARD, scrollCameraSpeed);
	}
}

void processMovement()
{

	if (pressedKeys[GLFW_KEY_D]) {
		if (carRotation < 1.0f) {
			carRotation += 0.01;
		}
	}

	if (pressedKeys[GLFW_KEY_A]) {
		if (carRotation > -1.0f) {
			carRotation -= 0.01;
		}
	}
	

	if (pressedKeys[GLFW_KEY_W]) {
		if ((carLocation.x < -12.5f || carLocation.x > 15.5f || carLocation.z < -11.0f || carLocation.z > 16) && cade == false) {
			cade = true;
		}
		else if ((carLocation.x <= -5.7f) ||
			(carLocation.x >= 9.0f ) ||
			(carLocation.z <= -5.2f && (carLocation.x > -5.7f || carLocation.x < 4)) ||
			(carLocation.z >= 9.0f && (carLocation.x > -5.7f || carLocation.x < 4)) && cade == false) {
			if (followCar == GL_TRUE) {
				yaw = glm::degrees(carXYZ);
				myCamera.setDirection(carLocation, glm::radians(pitch), carXYZ);

			}
			carFront.x += carSpeed * cos(carXYZ + carRotation);
			carFront.z += carSpeed * sin(carXYZ + carRotation);
			carBack.x += carSpeed * cos(carXYZ);
			carBack.z += carSpeed * sin(carXYZ);
			carLocation.x = (carFront.x + carBack.x) / 2;
			carLocation.z = (carFront.z + carBack.z) / 2;
			carXYZ = atan2((carFront.z - carBack.z), (carFront.x - carBack.x));
		}
	}

	if (pressedKeys[GLFW_KEY_S]) {
		if (followCar == GL_TRUE) {
			yaw = glm::degrees(carXYZ);
			myCamera.setDirection(carLocation, glm::radians(pitch), carXYZ);
		}
		carFront.x -= carSpeed * cos(carXYZ + carRotation);
		carFront.z -= carSpeed * sin(carXYZ + carRotation);
		carBack.x -= carSpeed * cos(carXYZ);
		carBack.z -= carSpeed * sin(carXYZ);
		carLocation.x = (carFront.x + carBack.x) / 2;
		carLocation.z = (carFront.z + carBack.z) / 2;
		carXYZ = atan2((carFront.z - carBack.z), (carFront.x - carBack.x));
	}

	if (pressedKeys[GLFW_KEY_Z]) {
		if (followCar == GL_TRUE) {
			followCar = GL_FALSE;
		}
		else {
			followCar = GL_TRUE;
		}
	}

	if (pressedKeys[GLFW_KEY_Q]) {
		angle += 0.1f;
		if (angle > 360.0f)
			angle -= 360.0f;
	}

	if (pressedKeys[GLFW_KEY_E]) {
		angle -= 0.1f;
		if (angle < 0.0f)
			angle += 360.0f;
	}

	if (pressedKeys[GLFW_KEY_T]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_G]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_F]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_H]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_J]) {

		lightAngle += 0.3f;
		if (lightAngle > 360.0f)
			lightAngle -= 360.0f;
		glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDirTr));
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle -= 0.3f; 
		if (lightAngle < 0.0f)
			lightAngle += 360.0f;
		glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
		myCustomShader.useShaderProgram();
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDirTr));
	}	

	if (pressedKeys[GLFW_KEY_C]) {
		if (computeFog != 0) {
			computeFog = 0.0f;
		}
		else {
			computeFog = 1.0f;
		}
		myCustomShader.useShaderProgram();
		glUniform1f(computeFogLoc, computeFog);
	}

	if (pressedKeys[GLFW_KEY_X]) {
		if (rain == false) {
			rain = true;
		}
		else {
			rain = false;
		}
	}
	if (pressedKeys[GLFW_KEY_P]) {
		if (night == false) {
			night = true;
		}
		else {
			night = false;
		}
	}

	if (pressedKeys[GLFW_KEY_V]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (pressedKeys[GLFW_KEY_B]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (pressedKeys[GLFW_KEY_N]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}

	if (pressedKeys[GLFW_KEY_M]) {
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
	}

}

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	//for Mac OS X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwMakeContextCurrent(glWindow);

	glfwWindowHint(GLFW_SAMPLES, 4);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	glfwSetScrollCallback(glWindow, scrollCallback);
    //glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	//glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initFBOs()
{
	//generate FBO ID
	glGenFramebuffers(1, &shadowMapFBO);

	//create depth texture for FBO
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//attach texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

glm::mat4 computeLightSpaceTrMatrix()
{
	const GLfloat near_plane = 1.0f, far_plane = 20.0f;
	glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);

	glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
	glm::mat4 lightView = glm::lookAt(lightDirTr, myCamera.getCameraTarget(), glm::vec3(0.0f, 1.0f, 0.0f));

	return lightProjection * lightView;
}


void initModels()
{
	myModel = gps::Model3D("objects/nanosuit/nanosuit.obj", "objects/nanosuit/");
	ground = gps::Model3D("objects/ground/ground.obj", "objects/ground/");
	town = gps::Model3D("objects/town/old_town_block.obj", "objects/town/");
	car = gps::Model3D("objects/car/body.obj", "objects/car/");
	carLeft = gps::Model3D("objects/car/leftWheel.obj", "objects/car/");
	carRight = gps::Model3D("objects/car/rightWheel.obj", "objects/car/");
	tree = gps::Model3D("objects/tree/BroadLeafStraightTrunk.obj", "objects/tree/");
	fountain = gps::Model3D("objects/fountain/fountain.obj", "objects/fountain/");
	bench = gps::Model3D("objects/bench/Bench.obj", "objects/bench/");
	stone = gps::Model3D("objects/stone/stone.obj", "objects/stone/");
	lamp = gps::Model3D("objects/lamp/lamp.obj", "objects/lamp/");
	boy = gps::Model3D("objects/boy/boy.obj", "objects/boy/");
	boy2 = gps::Model3D("objects/boy2/boy2.obj", "objects/boy2/");
	ball = gps::Model3D("objects/ball/ball.obj", "objects/ball/");
	dog = gps::Model3D("objects/dog/dog.obj", "objects/dog/");
	water = gps::Model3D("objects/water/water.obj", "objects/water/");

	drop = gps::Model3D("objects/drop/drop.obj", "objects/drop/");
	lightCube = gps::Model3D("objects/cube/cube.obj", "objects/cube/");


	glCreateDrops();
}

void initCar() {

	carLocation = glm::vec3(-8.0f, -0.95f, 0.0f);
	carFront.x = carLocation.x + 0.5 * cos(carXYZ);
	carFront.z = carLocation.z + 0.5 * sin(carXYZ);
	carBack.x = carLocation.x - 0.5 * cos(carXYZ);
	carBack.z = carLocation.z - 0.5 * sin(carXYZ);
}

void initShaders()
{
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
	depthMapShader.loadShader("shaders/simpleDepthMap.vert", "shaders/simpleDepthMap.frag");
	skyBoxShader.loadShader("shaders/skyBoxShader.vert", "shaders/skyBoxShader.frag");

}

void initSkyBox() {
	faces.push_back("skybox/right.tga");
	faces.push_back("skybox/left.tga");
	faces.push_back("skybox/top.tga");
	faces.push_back("skybox/bottom.tga");
	faces.push_back("skybox/back.tga");
	faces.push_back("skybox/front.tga");
	mySkyBox.Load(faces);
}

void goodNight() {
	//set light color
	if (night == true) {
		for (float i = 0; i < 1.0f; i+=0.001) {

			for (int j = 0; j < 1000; j++) {
				lightColor = glm::vec3(1.0f - i, 1.0f - i, 1.0f - i); //white light
				lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
				glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
			}
		}
	}
	else {
		for (float i = 0; i < 1.0f; i += 0.001) {
			for (int j = 0; j < 1000; j++) {
				lightColor = glm::vec3(0.0f+i, 0.0f+i, 0.0f+i); //white light
				lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
				glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
			}
		}
	}
}

void initUniforms()
{
	myCustomShader.useShaderProgram();

	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");

	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	
	lightDirMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDirMatrix");

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));	



	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 5.0f, 5.0f);
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

	
	//set the light direction (direction towards the light) lamp1
	//lightDirLamp1 = glm::vec3(1.0f, 0.12f, -1.7f);
	lightPosLamp1 = glm::vec3(1.0f, 0.12f, -1.7f);
	lightPosLocLamp1 = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosLamp1");
	glUniform3fv(lightPosLocLamp1, 1, glm::value_ptr(lightPosLamp1));

	
	//set the light direction (direction towards the light) lamp2
	lightPosLamp2 = glm::vec3(5.5f, 0.12f, 1.3f);
	lightPosLocLamp2 = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosLamp2");
	glUniform3fv(lightPosLocLamp2, 1, glm::value_ptr(lightPosLamp2));
	
	//set the light direction (direction towards the light) lamp3
	lightPosLamp3 = glm::vec3(2.5f, 0.12f, 6.0f);
	lightPosLocLamp3 = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosLamp3");
	glUniform3fv(lightPosLocLamp3, 1, glm::value_ptr(lightPosLamp3));

	//set the light direction (direction towards the light) lamp4
	lightPosLamp4 = glm::vec3(-2.1f, 0.12f, 3.2f);
	lightPosLocLamp4 = glGetUniformLocation(myCustomShader.shaderProgram, "lightPosLamp4");
	glUniform3fv(lightPosLocLamp4, 1, glm::value_ptr(lightPosLamp4));


	//set the light direction lamp1
	lightSpotLamp1 = glm::vec3(1.0f, -1.4f, 1.0f);
	lightSpotLocLamp1 = glGetUniformLocation(myCustomShader.shaderProgram, "lightSpotLamp1");
	glUniform3fv(lightSpotLocLamp1, 1, glm::value_ptr(lightSpotLamp1));

	//set the light direction lamp2
	lightSpotLamp2 = glm::vec3(7.5f, -1.4f, 1.3f);
	lightSpotLocLamp2 = glGetUniformLocation(myCustomShader.shaderProgram, "lightSpotLamp2");
	glUniform3fv(lightSpotLocLamp2, 1, glm::value_ptr(lightSpotLamp2));

	//set the light direction lamp3
	lightSpotLamp3 = glm::vec3(2.5f, -1.4f, 4.0f);
	lightSpotLocLamp3 = glGetUniformLocation(myCustomShader.shaderProgram, "lightSpotLamp3");
	glUniform3fv(lightSpotLocLamp3, 1, glm::value_ptr(lightSpotLamp3));

	//set the light direction lamp4
	lightSpotLamp4 = glm::vec3(-0.1f, -1.4f, 3.2f);
	lightSpotLocLamp4 = glGetUniformLocation(myCustomShader.shaderProgram, "lightSpotLamp3");
	glUniform3fv(lightSpotLocLamp4, 1, glm::value_ptr(lightSpotLamp4));

	goodNight();

	//set light color
	lightColor2 = glm::vec3(1.0f, 1.0f, 0.8f); //blue light
	lightColorLoc2 = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor2");
	glUniform3fv(lightColorLoc2, 1, glm::value_ptr(lightColor2));

	//set whether to compute fog
	computeFogLoc = glGetUniformLocation(myCustomShader.shaderProgram, "computeFogEnable");
	glUniform1f(computeFogLoc, computeFog);

	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


}

void renderScene2(gps::Shader depthMapShader) {

	//create model matrix for town1
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-7.5f, -2.7f, -6.5f));
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	town.Draw(depthMapShader);

	//create model matrix for town2
	model = glm::translate(glm::mat4(1.0f), glm::vec3(10.2f, -2.7f, -7.6f));
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	town.Draw(depthMapShader);

	//create model matrix for town3
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-6.5f, -2.7f, 11.2f));
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	town.Draw(depthMapShader);

	//create model matrix for town4
	model = glm::translate(glm::mat4(1.0f), glm::vec3(11.3f, -2.7f, 10.2f));
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	town.Draw(depthMapShader);


	//create model matrix for tree1
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree2
	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree3
	model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree4
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree5
	model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree6
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree7
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree8
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree9
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, -1.0f, 6.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree10
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.8f, -1.0f, 7.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);
	
	//create model matrix for tree11
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.8f, -1.0f, 8.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree12
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.8f, -1.0f, 10.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree13
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.8f, -1.0f, 11.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree14
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.3f, -1.0f, 13.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree15
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree16
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree17
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree18
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree19
	model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree20
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree21
	model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree22
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 13.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree23
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 12.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree24
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 10.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree25
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 9.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree26
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 7.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);

	//create model matrix for tree27
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 6.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	tree.Draw(depthMapShader);


	//create model matrix for fountain
	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -0.8f, 2.4f));
	model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	fountain.Draw(depthMapShader);

	//create model matrix for bench1
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.75f, -0.8f, -1.7f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	bench.Draw(depthMapShader);


	//create model matrix for bench2
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.9f, -0.8f, 6.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	bench.Draw(depthMapShader);

	//create model matrix for bench3
	model = glm::translate(glm::mat4(1.0f), glm::vec3(5.5f, -0.8f, 2.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	bench.Draw(depthMapShader);

	//create model matrix for bench4
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.8f, 2.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	bench.Draw(depthMapShader);

	//create model matrix for lamp1
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, -1.7f));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	lamp.Draw(depthMapShader);

	//create model matrix for lamp2
	model = glm::translate(glm::mat4(1.0f), glm::vec3(5.5f, 0.5f, 1.3f));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	lamp.Draw(depthMapShader);

	//create model matrix for lamp3
	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 0.5f, 6.0f));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	lamp.Draw(depthMapShader);

	//create model matrix for lamp4
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.1f, 0.5f, 3.2f));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));
	lamp.Draw(depthMapShader);

	
	//create model matrix for boy
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, -0.1f));
	model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1, 0));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	boy.Draw(depthMapShader);

	//create model matrix for boy2
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.15f, -1.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	boy2.Draw(depthMapShader);


	//create model matrix for ball
	model = glm::translate(glm::mat4(1.0f), glm::vec3(deplasareX, -0.9f, deplasareZ));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	model = glm::rotate(model, glm::radians(ballRotation), glm::vec3(1, 0, 0));

	deplasareX += indiceX;
	deplasareZ += indiceZ;
	ballRotation += indiceRotation;

	if (deplasareX > 1.0 || deplasareZ > 1.1f)
	{
		deplasareX = 1.0f;
		deplasareZ = 1.0f;
		indiceX = -indiceX;
		indiceZ = -indiceZ;
		indiceRotation = -indiceRotation;
	}
	if (deplasareX < -1.0f || deplasareZ < -0.1f)
	{
		deplasareX = -1.0f;
		deplasareZ = 0.0f;
		indiceX = -indiceX;
		indiceZ = -indiceZ;
		indiceRotation = -indiceRotation;
	}
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	ball.Draw(depthMapShader);


	//create model matrix for dog
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -1.0f, -1.5f));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0, 0, 1));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	dog.Draw(depthMapShader);

	//create model matrix for dog
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.2f, -1.0f, -1.3f));
	model = glm::scale(model, glm::vec3(0.004f, 0.004f, 0.004f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0, 0, 1));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	dog.Draw(depthMapShader);


	
	//create model matrix for car
	model = glm::translate(glm::mat4(1.0f), carLocation);
	model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	car.Draw(depthMapShader);
	

	//create model matrix for ground
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//send model matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "model"),
		1,
		GL_FALSE,
		glm::value_ptr(model));

	ground.Draw(depthMapShader);

}

void renderScene3(gps::Shader myCustomShader) {
	//create model matrix for town1
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-7.5f, -2.7f, -6.5f));
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	//send model matrix data to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//create normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	town.Draw(myCustomShader);

	//create model matrix for town2
	model = glm::translate(glm::mat4(1.0f), glm::vec3(10.2f, -2.7f, -7.6f));
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	town.Draw(myCustomShader);

	//create model matrix for town3
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-6.5f, -2.7f, 11.2f));
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	town.Draw(myCustomShader);

	//create model matrix for town4
	model = glm::translate(glm::mat4(1.0f), glm::vec3(11.3f, -2.7f, 10.2f)); 
	model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	town.Draw(myCustomShader);


	//create model matrix for tree1
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	tree.Draw(myCustomShader);

	//create model matrix for tree2
	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree3
	model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree4
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree5
	model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree6
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree7
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree8
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -1.0f, 5.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree9
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, -1.0f, 6.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree10
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.8f, -1.0f, 7.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree11
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.8f, -1.0f, 8.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree12
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.8f, -1.0f, 10.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree13
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.8f, -1.0f, 11.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree14
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.3f, -1.0f, 13.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree15
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree16
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree17
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree18
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree19
	model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree20
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree21
	model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -1.0f, 13.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree22
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 13.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree23
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 12.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree24
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 10.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree25
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 9.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree26
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 7.5f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);

	//create model matrix for tree27
	model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 6.0f));
	model = glm::scale(model, glm::vec3(0.012f, 0.012f, 0.012f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	tree.Draw(myCustomShader);


	//create model matrix for fountain
	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, -0.8f, 2.4f));
	model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	fountain.Draw(myCustomShader);

	//create model matrix for bench1
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.75f, -0.8f, -1.7f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	bench.Draw(myCustomShader);

	//create model matrix for bench2face
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.9f, -0.8f, 6.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	bench.Draw(myCustomShader);

	//create model matrix for bench3left
	model = glm::translate(glm::mat4(1.0f), glm::vec3(5.5f, -0.8f, 2.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	bench.Draw(myCustomShader);

	//create model matrix for bench4right
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.8f, 2.5f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	bench.Draw(myCustomShader);

	//create model matrix for stone
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.75f, -0.9f, -1.7f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	stone.Draw(myCustomShader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.75f, -0.9f, -0.8f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.75f, -0.9f, 0.2f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.75f, -0.9f, 1.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.85f, -0.9f, 2.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.85f, -0.9f, 3.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.85f, -0.9f, 4.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.85f, -0.9f, 5.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.5f, -0.9f, 2.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, -0.9f, 2.1f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, -0.9f, 2.2f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, -0.9f, 2.3f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, -0.9f, 2.4f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(4.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.9f, 2.45f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(3.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -0.9f, 2.5f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(3.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.9f, 2.55f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(2.0f), glm::vec3(0, 0, 1));
	model = glm::scale(model, glm::vec3(0.003f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	stone.Draw(myCustomShader);

	//create model matrix for lamp1
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, -1.7f));
	//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	lamp.Draw(myCustomShader);

	//create model matrix for lamp2back
	model = glm::translate(glm::mat4(1.0f), glm::vec3(5.5f, 0.5f, 1.3f));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	lamp.Draw(myCustomShader);

	//create model matrix for lamp3left
	model = glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 0.5f, 6.0f));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	lamp.Draw(myCustomShader);

	//create model matrix for lamp4right
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.1f, 0.5f, 3.2f));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	lamp.Draw(myCustomShader);

	//create model matrix for boy
	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, -0.1f));
	model = glm::scale(model, glm::vec3(0.09f, 0.09f, 0.09f));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0, 1, 0));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	boy.Draw(myCustomShader);

	//create model matrix for boy2
	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.15f, -1.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
	model = glm::rotate(model, glm::radians(-120.0f), glm::vec3(0, 1, 0));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	boy2.Draw(myCustomShader);


	//create model matrix for ball


	//glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram,"model"),1,GL_FALSE, glm::value_ptr(glm::mat4(0.0f)));
	model = glm::translate(glm::mat4(1.0f), glm::vec3(deplasareX, -0.9f, deplasareZ));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	model = glm::rotate(model, glm::radians(ballRotation), glm::vec3(1, 0, 0));

	deplasareX += indiceX;
	deplasareZ += indiceZ;
	ballRotation += indiceRotation;

	if (deplasareX > 1.0 || deplasareZ > 1.1f )
	{
		deplasareX = 1.0f;
		deplasareZ = 1.0f;
		indiceX = -indiceX;
		indiceZ = -indiceZ;
		indiceRotation = -indiceRotation;
	}
	if (deplasareX < -1.0f || deplasareZ < -0.1f)
	{
		deplasareX = -1.0f;
		deplasareZ = 0.0f;
		indiceX = -indiceX;
		indiceZ = -indiceZ;
		indiceRotation = -indiceRotation;
	}
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	ball.Draw(myCustomShader);


	//create model matrix for dog
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, -1.0f, -1.5f));
	model = glm::scale(model, glm::vec3(0.007f, 0.007f, 0.007f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0, 0, 1));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	dog.Draw(myCustomShader);

	//create model matrix for dog
	model = glm::translate(glm::mat4(1.0f), glm::vec3(4.2f, -1.0f, -1.3f));
	model = glm::scale(model, glm::vec3(0.004f, 0.004f, 0.004f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0, 0, 1));
	//send model matrix data to shader	
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//compute normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	dog.Draw(myCustomShader);



	if (cade == false) {
		//create model matrix for taxi body and send model matrix to shader
		model = glm::translate(glm::mat4(1), carLocation);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, -carXYZ, glm::vec3(0, 1, 0));
		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		car.Draw(myCustomShader);

		//create model matrix for left wheel and send model matrix to shader
		model = glm::translate(glm::mat4(1), carLocation);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, -carXYZ, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(0.355f, 0.0f, -0.1f));
		model = glm::rotate(model, -carRotation, glm::vec3(0, 1, 0));
		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		carLeft.Draw(myCustomShader);

		//create model matrix for right wheel and send model matrix to shader
		model = glm::translate(glm::mat4(1), carLocation);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, -carXYZ, glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(0.355f, 0.0f, 0.1f));
		model = glm::rotate(model, -carRotation, glm::vec3(0, 1, 0));
		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		carRight.Draw(myCustomShader);
	}
	else {
		model = glm::translate(glm::mat4(1), carLocation);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, glm::radians(indiceCarRotation), glm::vec3(1, 1, 1));

		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		car.Draw(myCustomShader);
		
		indiceCarRotation += indiceCadere;
		carLocation.y -= 0.01f;
	}

	glUpdateDrops();
	glDrawDrops();
	

	if (rain == true) {
		for (int i = 0; i < noOfDroplets; i++)
		{


			model = glm::translate(glm::mat4(1.0f), glm::vec3(dropletsX[i], dropletsY[i], dropletsZ[i]));
			model = glm::scale(model, glm::vec3(dropScale, dropScale, dropScale));
			//send model matrix data to shader	
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			//compute normal matrix
			normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
			//send normal matrix data to shader
			glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
			drop.Draw(myCustomShader);
			dropletsY[i] -= 1;
			if (dropletsY[i] < -40)
			{
				dropletsX[i] = getRand(-5.0, 5.0) * 4;
				dropletsY[i] = getRand(0, 100);
				dropletsZ[i] = getRand(-5.0, 5.0) * 2;
			}
		}
	}



	//create model matrix for taxi body and send model matrix to shader
	model = glm::translate(glm::mat4(1), glm::vec3(watermovement, waterhight, waterZ));
	model = glm::scale(model, glm::vec3(40.05f, 40.05f, 50.05f));
	//model = glm::rotate(model, -carXYZ, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	water.Draw(myCustomShader);

	if (waterFlag == 0 && watermovement < 10) {
		if (sus == 1) {
			watermovement += 0.1;
			waterZ += 0.2;
			waterhight += 0.03;
			sus = 0;
		}
		else {
			watermovement += 0.1;
			waterZ -= 0.2;
			waterhight -= 0.03;
			sus = 1;
		}
	}
	else
		waterFlag = 1;
	if (waterFlag == 1 && watermovement > -10) {
		if (sus == 0) {
			watermovement -= 0.1;
			waterZ -= 0.2;
			waterhight -= 0.03;
			sus = 1;
		}
		else {

			watermovement -= 0.1;
			waterZ += 0.2;
			waterhight += 0.03;
			sus = 0;
		}

	}
	else
		waterFlag = 0;


	//create model matrix for ground
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	//send model matrix data to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	//create normal matrix
	normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	//send normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	ground.Draw(myCustomShader);

}

void renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	processMovement();	

	//render the scene to the depth buffer (first pass)

	depthMapShader.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));
		
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	renderScene2(depthMapShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//render the scene (second pass)

	myCustomShader.useShaderProgram();

	//send lightSpace matrix to shader
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));


	//send view matrix to shader
	view = myCamera.getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "view"),
		1,
		GL_FALSE,
		glm::value_ptr(view));	

	//compute light direction transformation matrix
	lightDirMatrix = glm::mat3(glm::inverseTranspose(view));
	//send lightDir matrix data to shader
	glUniformMatrix3fv(lightDirMatrixLoc, 1, GL_FALSE, glm::value_ptr(lightDirMatrix));

	glViewport(0, 0, retina_width, retina_height);
	myCustomShader.useShaderProgram();

	//bind the depth map
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);
	

	renderScene3(myCustomShader);
	goodNight();

	
	//draw a white cube around the light

	lightShader.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	model = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, lightDir);
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	lightCube.Draw(lightShader);
	
	//draw a white cube around the light1
	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view)); 

	//model = glm::rotate(model, glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::translate(glm::mat4(1.0f) , glm::vec3(1.0f, 0.12f, -1.7f));
	model = glm::translate(glm::mat4(1.0f), lightPosLamp1);
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	lightCube.Draw(lightShader);

	//draw a white cube around the light2
	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	//model = glm::rotate(model, glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(glm::mat4(1.0f), lightPosLamp2);
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	lightCube.Draw(lightShader);

	//draw a white cube around the light3
	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	//model = glm::rotate(model, glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(glm::mat4(1.0f), lightPosLamp3);
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	lightCube.Draw(lightShader);

	//draw a white cube around the light4
	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	//model = glm::rotate(model, glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(glm::mat4(1.0f), lightPosLamp4);
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	lightCube.Draw(lightShader);


	skyBoxShader.useShaderProgram();

	view = myCamera.getViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(skyBoxShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(skyBoxShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	mySkyBox.Draw(skyBoxShader, view, projection);

}

int main(int argc, const char * argv[]) {

	initOpenGLWindow();
	initOpenGLState();
	initFBOs();
	initModels();
	initCar();
	initShaders();
	initSkyBox();
	initUniforms();	
	glCheckError();
	while (!glfwWindowShouldClose(glWindow)) {
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	//close GL context and any other GLFW resources
	glfwTerminate();

	return 0;
}
