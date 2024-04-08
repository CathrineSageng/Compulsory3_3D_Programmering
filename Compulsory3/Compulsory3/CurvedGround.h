#ifndef CURVEDGROUND_H
#define CURVEDGROUND_H

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include<iostream>

using namespace std;

class CurvedGround
{
public:
    CurvedGround();
    ~CurvedGround();
    void loadCurvedGround(const char* filename);

    GLuint getVAO() const;
    GLuint getVBO() const;
    GLuint getEBO() const;
    GLsizei getIndexCount() const;
    void readDataFromFile(const char* filename, vector<GLfloat>& vertices, vector<GLuint>& indices);

private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLsizei indexCount;

    
};

#endif
