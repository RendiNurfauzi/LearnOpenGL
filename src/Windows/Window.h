#ifndef WINDOW_CLASS_H
#define WINDOW_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>


class Window {

public:

	GLFWwindow* handle;
	int width, height;
	Window(int width, int height, const char* title);

	bool ShouldClose();
	void SwapBuffers();
	void PollEvents();
	void Clear(float r, float g, float b, float a);
	void Delete();
};
#endif // !WINDOW_CLASS_H
