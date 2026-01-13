#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>


#include "Shaders/shaderClass.h"
#include "Buffers/VAO.h"
#include "Buffers/VBO.h"
#include "Buffers/EBO.h"

int main() 
{
	// Melakukan inialisasi GLFW
	glfwInit();

	// Versi OpenGL yang dipakai OpengGL 3.3/ Major, Minor
	// Menentukan versi utama OpenGL yang di gunakan
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	// Menentukan versi minor OpenGL yang digunakan
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Menggunakan Core Profile OpenGL
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Membuat vertex array berbentuk segitiga
	GLfloat vertices[] = {
		//					COORDINATES					//				COLORS					//
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,		0.8f,	0.3f,	0.02f, // Lower Left Corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,			0.8f,	0.3f,	0.02f, // Lower Right Corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,		1.0f,	0.6f,	0.32f, // Upper Corner
		-0.25f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,	0.9f,	0.45f,	0.17f, // Inner Left
		0.25f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,		0.9f,	0.45f,	0.17f, // Inner Right
		0.0f, -0.5 * float(sqrt(3)) / 3, 0.0f,			0.8f,	0.3f,	0.02f, // Inner Down
	};

	GLuint indices[] = {
		0, 3, 5, // Lower Left Triangle
		3, 2, 4, // Lower Right Triangle
		5, 4, 1, // Upper Triangle
	};

	// Membuat object Window dengan GLFWwindow dengan resolusi 1280, 720, Nama Renderer Engine, Windowed, Tanpa shared OpenGL context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Renderer Engine", NULL, NULL);

	// Fallback jika window gagal dibuat
	if (window == NULL) {
		std::cout << "FAILDED TO CREATE A GLFW WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Melakukan set context object window, yang digunakan sekarang
	glfwMakeContextCurrent(window);

	// Mengambil fungsi OpenGL menggunakan GLAD
	gladLoadGL();

	// Menentukan area render di viewport
	glViewport(0, 0, 1280, 720);

	Shader shader(
		"src/Shaders/file/default.vert",
		"src/Shaders/file/default.frag"
		);

	// Membuat Vertex Array Object
	VAO vao;
	vao.Bind();

	// Membuat Vertex Buffer Object dan upload data vertex
	VBO vbo(vertices, sizeof(vertices));

	// Membuat Element Buffer Object dan upload data index
	EBO ebo(indices, sizeof(indices));

	// Menghubungkan VBO ke VAO (layout location = 0)
	vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	vao.LinkAttribute(vbo, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// Unbind VAO
	vao.Unbind();
	vbo.Unbind();

	GLuint uniID = glGetUniformLocation(shader.ID, "scale");

	// Selagi object window tidak di ditutup
	while (!glfwWindowShouldClose(window)) {
		// Backbuffer menentukan warna yang akan digunakan saat glClear didalam looping window
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// Membersihkan color buffer dengan warna glClearColor yang telah ditentukan didalam looping window
		glClear(GL_COLOR_BUFFER_BIT);
		// Menggunakan shader program yang telah disiapkan
		shader.Active();
		glUniform1f(uniID, 0.5f);
		// Melakukan binding Vertex Array Object
		vao.Bind();
		// Menggambar Array (Primitif Yang Ingin Digunakan, Index awal vertex, Jumlah vertex yang di gambar)
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// Menukar buffer pada frame setiap looping
		glfwSwapBuffers(window);
		// Melakukan prosess semua event yang terjadi
		glfwPollEvents();
	}

	// Penghapusan Object VAO
	vao.Delete();
	// Ppenghapusan Object VBO
	vbo.Delete();
	// Penghapusan Object EBO
	ebo.Delete();
	// Penghapusan Shader Program
	shader.Delete();

	// GLFW Hancurkan object window
	glfwDestroyWindow(window);

	// GLFW dimatikan
	glfwTerminate();

	// Mengembalikan nilai 0
	return 0;
}

//disini penjelasan script nya