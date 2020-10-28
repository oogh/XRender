//
// Created by Oogh on 2020/10/26.
//

#include "XTriangle.hpp"
#include "XShader.hpp"

const float vertices[] = {
        // Positions         // Colors
        0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  // Bottom Left
        0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // Top
};

const char* vertexFilePath = "/Users/oogh/Workspace/Resources/shaders/triangle.vs";
const char* fragmentFilePath = "/Users/oogh/Workspace/Resources/shaders/triangle.fs";

XTriangle::XTriangle() {
    mShader = std::make_unique<XShader>(vertexFilePath, fragmentFilePath);
    glGenVertexArraysAPPLE(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArrayAPPLE(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

//    glBindVertexArrayAPPLE(0); // Unbind VAO
}

XTriangle::~XTriangle() {

}

void XTriangle::draw() {
    mShader->use();
    glBindVertexArrayAPPLE(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArrayAPPLE(0);
}
