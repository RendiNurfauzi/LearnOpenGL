#include"shaderClass.h"

std::string get_file_contents(const char* filename) {

	std::ifstream in(filename, std::ios::binary);
	if (in) {

	std::string contents;
	in.seekg(0, std::ios::end);
	contents.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();
	return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Membuat object shader bertipe vertex shader (belum memiliki source)
	GLuint _vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Mengasosiasikan source code GLSL kedalam shader object
	glShaderSource(_vertexShader, 1, &vertexSource, NULL);
	// Melakukan kompilasi pada _vertexShader
	glCompileShader(_vertexShader);

	compileErrors(_vertexShader, "VERTEX");

	// Membuat object shadr bertipe fragment shader (belum memiliki source)
	GLuint _fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Mengasosiasikan source code GLSL kedalam shader object
	glShaderSource(_fragmentShader, 1, &fragmentSource, NULL);
	// Melakukan kompilasi pada _fragmentShader
	glCompileShader(_fragmentShader);
	compileErrors(_fragmentShader, "FRAGMENT");

	// Membuat shader program
	ID = glCreateProgram();
	// Memasukan _vertexShader kedalam _shaderProgram
	glAttachShader(ID, _vertexShader);
	// Memasukan _fragmentShader kedalam _shaderProgram
	glAttachShader(ID, _fragmentShader);

	// Melakukan Link Shader kedalam program
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	// Pembersihan _vertexShader
	glDeleteShader(_vertexShader);
	// Pembersihan _fragmentShader
	glDeleteShader(_fragmentShader);
}

void Shader::Active() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type) {

	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM") {

		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);

		if (hasCompiled == GL_FALSE) {

			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILE_ERROR" << type << "\n" << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);

		if (hasCompiled == GL_FALSE) {

			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR" << type << "\n" << std::endl;
		}
	}
}