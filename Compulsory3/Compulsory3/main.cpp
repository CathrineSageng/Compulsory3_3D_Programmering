#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <vector>

#include"ShaderClass.h"

using namespace std;

const GLuint WIDTH = 1000, HEIGHT = 1000;
const GLfloat cameraSpeed = 0.001f;
glm::vec3 cubePosition = glm::vec3(0.0f, 0.5f, 0.0f); // Initial position of the cube


// Camera settings
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool firstMouse = true;
bool keys[1024];

// Mouse movement callback function
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// Keyboard input callback function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    GLfloat cameraSpeed = 0.05f;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Cube with Camera", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    //Generates Shader object using shaders defualt.vert and default.frag
    Shader shaderProgram("default.vert", "default.frag");

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);


    GLfloat groundVertices[] = {
    0.0f, -2.0f, 0.0f,       0.0f, 0.5f, 0.0f,
    0.0f, -2.0f, 2.0f,       0.0f, 0.5f, 0.0f,
    0.0f, -2.0f, 8.0f,       0.0f, 0.5f, 0.0f,
    2.0f, -0.0f, 6.0f,       0.0f, 0.5f, 0.0f,
    3.0f, -2.0f, 0.0f,       0.0f, 0.5f, 0.0f,
    4.0f, -2.0f, 4.0f,       0.0f, 0.5f, 0.0f,
    5.0f, -2.0f, 2.0f,       0.0f, 0.5f, 0.0f,
    5.0f, -2.0f, 8.0f,       0.0f, 0.5f, 0.0f,
    7.0f, -0.0f, 5.0f,       0.0f, 0.5f, 0.0f,
    8.0f, -2.0f, 0.0f,       0.0f, 0.5f, 0.0f,
    8.0f, -2.0f, 1.0f,       0.0f, 0.5f, 0.0f,
    8.0f, -2.0f, 8.0f,       0.0f, 0.5f, 0.0f,
    };

    GLuint groundIndices[] = {
    0, 4, 1,
    1, 4, 6,
    1, 6, 5,
    1, 5, 3,
    1, 3, 2,
    4, 10, 6,
    4, 9, 10,
    8, 10, 11,
    5, 6, 10,
    5, 10, 8,
    7, 8, 11,
    5, 8, 7,
    3, 5, 7,
    2, 3, 7
  };

    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundIndices), groundIndices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        GLfloat deltaTime = glfwGetTime();
        glfwSetTime(0.0f);

        // Handle keyboard input for camera movement
        if (keys[GLFW_KEY_W])
            cameraPos += cameraSpeed * cameraFront;
        if (keys[GLFW_KEY_S])
            cameraPos -= cameraSpeed * cameraFront;
        if (keys[GLFW_KEY_A])
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (keys[GLFW_KEY_D])
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


        // Clear the color and depth buffers
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader program
        shaderProgram.Activate();

        // View matrix
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // Pass transformation matrices to shader
        GLint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram.ID, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 42, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(shaderProgram.ID);
        glm::mat4 model = glm::mat4(1.0f); // Identity matrix
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}