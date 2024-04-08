#ifndef CHARACTER_H
#define CHARACTER_H

#include <glad/glad.h>
#include <glm/glm.hpp>


class Character
{
public:
    Character(const glm::vec3& position = glm::vec3(0.0f));
    ~Character();
    void drawCharacter();
    void setPosition(const glm::vec3& position);


private:
    GLuint VAO;
    GLuint VBO;
    glm::vec3 position_;
};

#endif
