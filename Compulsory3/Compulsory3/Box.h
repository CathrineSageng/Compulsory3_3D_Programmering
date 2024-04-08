#ifndef BOX_H
#define BOX_H

#include <glad/glad.h>
#include <glm/glm.hpp>


class Box
{
public:
    Box();
    ~Box();
    void drawBox();


private:
    GLuint VAO;
    GLuint VBO;
};

#endif
