#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <vector>
#include "../Buffers/VAO.h"
#include "../Buffers/EBO.h"
#include "../Shaders/shaderClass.h"
#include "../Math/EngineMath.h"

struct Vertex {
	EngineMath::Vec3 position;
	EngineMath::Vec3 color;
	EngineMath::Vec2 texUV;
};

class Mesh {

public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	VAO vao;

	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

	void Draw(Shader& shader);
};

#endif