#ifndef  CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"../Math/EngineMath.h"
#include"../Shaders/shaderClass.h"
#include"../Windows/Window.h"

class Camera {

public:
	EngineMath::Vec3 Position;
	EngineMath::Vec3 Orientation = EngineMath::Vec3(0.0f, 0.0f, -1.0f);
	EngineMath::Vec3 Up = EngineMath::Vec3(0.0f, 1.0f, 0.0f);
	EngineMath::Vec3 CurrentVelocity = EngineMath::Vec3(0.0f, 0.0f, 0.0f);

	int width;
	int height;

	float speed = 2.5f;
	float sensitivity = 500.0f;
	float friction = 10.0f;

	bool firstClick = true;
	float pitch = 0.0f;
	float yaw = -90.0f;

	// Variabel untuk smoothing mouse
	float targetYaw = -90.0f;
	float targetPitch = 0.0f;
	float currentYaw = -90.0f;
	float currentPitch = 0.0f;

	float mouseSmoothing = 15.0f;

	Camera(int width, int height, EngineMath::Vec3 posiiton);

	void Matrix(float fovDeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
	void Inputs(Window* window, float deltaTime);
};

#endif // ! CAMERA_CLASS_H
