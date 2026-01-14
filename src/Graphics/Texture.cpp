#include"Texture.h"
#include<stb/stb_image.h>

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {

	type = texType;

	stbi_set_flip_vertically_on_load(true);
	int widthImg, heightImg, numColCh;

	unsigned char* bytes = stbi_load("Externals/Resource/cat.jpg", &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (bytes) {
		glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
		glGenerateMipmap(texType);
	}
	else {
		std::cerr << "Failed to load texture: " << image << std::endl;
	}

	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Active();
	glUniform1i(texUni, unit);
}

void Texture::Bind() {
	glBindTexture(type, ID);
}

void Texture::Unbind() {
	glBindTexture(type, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}