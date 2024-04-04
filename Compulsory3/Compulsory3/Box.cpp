#include "Box.h"

Box::Box()
{
    VBO = 0;
    VAO = 0;
    EBO = 0;

    // Define vertices of the cube
    GLfloat vertices[] = {
        // Positions                //Colors
    -0.25f, -0.25f, -0.25f,        1.0f, 0.5f, 0.5f,     //Orange
    0.25f, -0.25f, -0.25f,         1.0f, 0.5f, 0.5f,
    0.25f, 0.25f, -0.25f,          1.0f, 0.5f, 0.5f,
    0.25f, 0.25f, -0.25f,          1.0f, 0.5f, 0.5f,
    -0.25f, 0.25f, -0.25f,         1.0f, 0.5f, 0.5f,
    -0.25f, -0.25f, -0.25f,        1.0f, 0.5f, 0.5f,

    -0.25f, -0.25f, 0.25f,         0.0f, 1.0f, 1.0f,     //Light blue
    0.25f, -0.25f, 0.25f,          0.0f, 1.0f, 1.0f,
    0.25f, 0.25f, 0.25f,           0.0f, 1.0f, 1.0f,
    0.25f, 0.25f, 0.25f,           0.0f, 1.0f, 1.0f,
    -0.25f, 0.25f, 0.25f,          0.0f, 1.0f, 1.0f,
    -0.25f, -0.25f, 0.25f,         0.0f, 1.0f, 1.0f,

    -0.25f, 0.25f, 0.25f,          1.0f, 1.0f, 0.0f,     //Yellow
    -0.25f, 0.25f, -0.25f,         1.0f, 1.0f, 0.0f,
    -0.25f, -0.25f, -0.25f,        1.0f, 1.0f, 0.0f,
    -0.25f, -0.25f, -0.25f,        1.0f, 1.0f, 0.0f,
    -0.25f, -0.25f, 0.25f,         1.0f, 1.0f, 0.0f,
    -0.25f, 0.25f, 0.25f,          1.0f, 1.0f, 0.0f,

    0.25f, 0.25f, 0.25f,           1.0f, 0.0f, 1.0f,     //Pink
    0.25f, 0.25f, -0.25f,          1.0f, 0.0f, 1.0f,
    0.25f, -0.25f, -0.25f,         1.0f, 0.0f, 1.0f,
    0.25f, -0.25f, -0.25f,         1.0f, 0.0f, 1.0f,
    0.25f, -0.25f, 0.25f,          1.0f, 0.0f, 1.0f,
    0.25f, 0.25f, 0.25f,           1.0f, 0.0f, 1.0f,

    -0.25f, -0.25f, -0.25f,        0.0f, 1.0f, 1.0f,     //Light blue
    0.25f, -0.25f, -0.25f,         0.0f, 1.0f, 1.0f,
    0.25f, -0.25f, 0.25f,          0.0f, 1.0f, 1.0f,
    0.25f, -0.25f, 0.25f,          0.0f, 1.0f, 1.0f,
    -0.25f, -0.25f, 0.25f,         0.0f, 1.0f, 1.0f,
    -0.25f, -0.25f, -0.25f,        0.0f, 1.0f, 1.0f,

    -0.25f, 0.25f, -0.25f,         1.0f, 0.5f, 0.5f,     //Orange
    0.25f, 0.25f, -0.25f,          1.0f, 0.5f, 0.5f,
    0.25f, 0.25f, 0.25f,           1.0f, 0.5f, 0.5f,
    0.25f, 0.25f, 0.25f,           1.0f, 0.5f, 0.5f,
    -0.25f, 0.25f, 0.25f,          1.0f, 0.5f, 0.5f,
    -0.25f, 0.25f, -0.25f,         1.0f, 0.5f, 0.5f,
    };

    GLuint Indices[] =
    {
    0, 1, 2,
    2, 3, 0,
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Box::~Box()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Box::drawBox()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
