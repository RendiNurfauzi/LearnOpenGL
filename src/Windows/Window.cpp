#include "Window.h"

Window::Window(int width, int height, const char* title) : width(width), height(height) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle = glfwCreateWindow(width, height, title, NULL, NULL);
    if (handle == NULL) {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(handle);
    gladLoadGL();
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
}

bool Window::ShouldClose() { return glfwWindowShouldClose(handle); }
void Window::SwapBuffers() { glfwSwapBuffers(handle); }
void Window::PollEvents() { glfwPollEvents(); }

void Window::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::Delete() {
    glfwDestroyWindow(handle);
    glfwTerminate();
}