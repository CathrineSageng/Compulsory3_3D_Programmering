#ifndef BOX_H
#define BOX_H

#include <glad/glad.h>
#include <glm/glm.hpp>


class Box
{
public:
    Box(glm::vec3 position, float width, float height, float depth);
    ~Box();
    void drawBox();
    glm::vec3 getMin() const; //minimum coordinates of the bounding box
    glm::vec3 getMax() const; //maximum coordinates of the bounding box

private:
    GLuint VAO;
    GLuint VBO;
    glm::vec3 minCoords; // Minimum coordinates of the bounding box
    glm::vec3 maxCoords; // Maximum coordinates of the bounding box
};

#endif

