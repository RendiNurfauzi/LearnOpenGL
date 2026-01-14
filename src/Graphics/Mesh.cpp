#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices) {
    this->vertices = vertices;
    this->indices = indices;

    vao.Bind();
    // VBO sekarang menggunakan struct Vertex, lebih rapi dan type-safe
    VBO vbo(vertices.data(), vertices.size() * sizeof(Vertex));
    EBO ebo(indices.data(), indices.size() * sizeof(GLuint));

    // Link attribute sesuai layout di shader (pos, color, uv)
    vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    vao.LinkAttribute(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    vao.LinkAttribute(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void Mesh::Draw(Shader& shader) {
    shader.Active();
    vao.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    vao.Unbind();
}