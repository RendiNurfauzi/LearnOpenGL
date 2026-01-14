#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

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
		// Positions        // Colors         // Texture Coords
		-0.5f, -0.5f, 0.0f,  1,0,0,  0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,  0,1,0,  0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,  0,0,1,  1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,  1,1,1,  1.0f, 0.0f
	};


	GLuint indices[] = {
		0, 2, 1,
		0, 3, 2
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
	vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao.LinkAttribute(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao.LinkAttribute(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// Unbind VAO
	vao.Unbind();
	vbo.Unbind();

	GLuint uniID = glGetUniformLocation(shader.ID, "scale");

	// Texture
	stbi_set_flip_vertically_on_load(true);
	int widthImg, heightImg, numColCh;
	unsigned char* bytes = stbi_load("Externals/Resource/cat.jpg", &widthImg, &heightImg, &numColCh, 0);


	GLuint texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Wrap & filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Sesuaikan format
	GLenum format = (numColCh == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, widthImg, heightImg, 0, format, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint tex0Uni = glGetUniformLocation(shader.ID, "tex0");
	shader.Active();
	glUniform1i(tex0Uni, 0);

	// Selagi object window tidak di ditutup
	while (!glfwWindowShouldClose(window)) {
		// Backbuffer menentukan warna yang akan digunakan saat glClear didalam looping window
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// Membersihkan color buffer dengan warna glClearColor yang telah ditentukan didalam looping window
		glClear(GL_COLOR_BUFFER_BIT);
		// Menggunakan shader program yang telah disiapkan
		shader.Active();
		glUniform1f(uniID, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Melakukan binding Vertex Array Object
		vao.Bind();
		// Menggambar Array (Primitif Yang Ingin Digunakan, Index awal vertex, Jumlah vertex yang di gambar)
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
	glDeleteTextures(1, &texture);

	// GLFW Hancurkan object window
	glfwDestroyWindow(window);

	// GLFW dimatikan
	glfwTerminate();

	// Mengembalikan nilai 0
	return 0;
}

//disini penjelasan script nya