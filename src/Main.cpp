#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Math/EngineMath.h"
#include "Windows/Window.h"
#include "Graphics/Texture.h"
#include "Graphics/Mesh.h"
#include "Shaders/shaderClass.h"
#include "World/Camera.h"

const unsigned int width = 1280;
const unsigned int height = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    Window window(1280, 720, "Renderer Engine");
    // --- Load Shader ---
    Shader shader("src/Shaders/file/default.vert", "src/Shaders/file/default.frag");

    // --- Data Piramida Testing (Dipindahkan ke Vector Vertex) ---
    // Format: { Position, Color, UV }
    std::vector<Vertex> vertices = {
        { EngineMath::Vec3(-0.5f, 0.0f,  0.5f), EngineMath::Vec3(1.0f, 0.0f, 0.0f), EngineMath::Vec2(0.0f, 0.0f) }, // 0: Kiri Depan
        { EngineMath::Vec3(-0.5f, 0.0f, -0.5f), EngineMath::Vec3(0.0f, 1.0f, 0.0f), EngineMath::Vec2(0.0f, 1.0f) }, // 1: Kiri Belakang
        { EngineMath::Vec3(0.5f, 0.0f, -0.5f), EngineMath::Vec3(0.0f, 0.0f, 1.0f), EngineMath::Vec2(1.0f, 1.0f) }, // 2: Kanan Belakang
        { EngineMath::Vec3(0.5f, 0.0f,  0.5f), EngineMath::Vec3(1.0f, 1.0f, 1.0f), EngineMath::Vec2(1.0f, 0.0f) }, // 3: Kanan Depan
        { EngineMath::Vec3(0.0f, 0.8f,  0.0f), EngineMath::Vec3(1.0f, 0.8f, 0.0f), EngineMath::Vec2(0.5f, 1.0f) }  // 4: Puncak
    };

    std::vector<GLuint> indices = {
        0, 2, 1, 0, 3, 2, // Alas
        0, 1, 4, 1, 2, 4, 2, 3, 4, 3, 0, 4 // Sisi Tegak
    };

    // --- Inisialisasi Mesh & Texture ---
    Mesh pyramid(vertices, indices);
    Texture catTex("Externals/Resource/cat.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
    catTex.texUnit(shader, "tex0", 0);

    glEnable(GL_DEPTH_TEST);

    Camera camera(width, height, EngineMath::Vec3(0.0f, 0.0f, 2.0f));

    // --- Main Loop ---
    while (!window.ShouldClose()) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        window.Clear(0.1f, 0.1f, 0.1f, 1.0f);
        shader.Active();

        camera.Inputs(&window, deltaTime);
        camera.Matrix(45.0f, 0.1f, 100.0f, shader, "camMatrix");

        catTex.Bind();
        pyramid.Draw(shader);

        window.SwapBuffers();
        window.PollEvents();
    }

    // --- Cleanup ---
    window.Delete();
    return 0;
}