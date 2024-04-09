#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <vector>

#include"ShaderClass.h"
#include"CurvedGround.h"
#include"Box.h"
#include"Camera.h"
#include "Character.h"

using namespace std;

const GLuint WIDTH = 1000, HEIGHT = 1000;
//Positions of the different objects 
glm::vec3 cube1Pos(1.0f, 0.0f, 2.0f);
glm::vec3 cube2Pos(4.0f, 0.25f, 2.0f);
glm::vec3 characterPos(0.25f, 0.25f, 0.25f);
glm::vec3 sunPos(4.0f, 2.0f, 2.0f);
glm::vec3 sunPos2(5.0f, 2.0f, 5.0f);
glm::vec3 groundPos(0.0f, -0.25f, 0.0f);


// Camera settings
Camera camera(glm::vec3(5.0f, 3.0f, 11.0f));
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f;

vector<Box> boxes;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, CurvedGround& curvedground, glm::vec3& characterPos);
unsigned int loadTexture(const char* path);
glm::vec3 calculateBarycentricCoordinates(const glm::vec3& point, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
float interpolateHeight(const glm::vec3& characterPos, CurvedGround& ground);
glm::vec3 calculateCharacterGroundPosition(const glm::vec3& characterPos, CurvedGround& ground);
bool detectCollision(const glm::vec3& characterPos, const glm::vec3& boxMin, const glm::vec3& boxMax);


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    camera.Pitch = -30.f;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Cube with Camera", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    //Generates Shader object using shaders defualt.vert and default.frag
    Shader shader("default.vert", "default.frag");
    Shader GroundShader("ground.vert", "ground.frag");

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    CurvedGround curvedground;
    curvedground.loadCurvedGround("data.txt");

    Box box1(cube1Pos, 1.0f, 1.0f, 1.0f);  //Position and size 
    Box box2(cube2Pos, 1.5f, 1.5f, 1.5f);  

    boxes.push_back(box1);
    boxes.push_back(box2);
    Character character; 

    // load textures (we now use a utility function to keep the code more organized)
  // -----------------------------------------------------------------------------
    unsigned int diffuseMap = loadTexture("Textures/container2.jpg");
    unsigned int specularMap = loadTexture("Textures/container2_specular.jpg");

    shader.use();
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, curvedground, characterPos);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate the character's position on the ground
        glm::vec3 characterGroundPos = calculateCharacterGroundPosition(characterPos, curvedground);

        // Update the character's position
        character.setPosition(characterGroundPos);

        shader.use();
        shader.setVec3("light.position", sunPos);
        shader.setVec3("viewPos", camera.Position);

        // Light properties
        shader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
        shader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
        shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        //shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
        shader.setFloat("material.shininess", 64.0f);

        /// View/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // World transformation
        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        //Draw a cube object 
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, cube1Pos);
        model = glm::scale(model, glm::vec3(0.5f)); // a smaller cube
        shader.setMat4("model", model);
        box1.drawBox();

        //Draw another cube object 
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, cube2Pos);
        shader.setMat4("model", model);
        box2.drawBox();

        GroundShader.use();
        GroundShader.setVec3("light.position", sunPos2);
        GroundShader.setVec3("viewPos", camera.Position);

        // Light properties
        GroundShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        GroundShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        GroundShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        GroundShader.setVec3("material.ambient", 0.0f, 0.3f, 0.0f);
        GroundShader.setVec3("material.diffuse", 0.0f, 0.8f, 0.0f);
        GroundShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
        GroundShader.setFloat("material.shininess", 32.0f);
        GroundShader.setMat4("projection", projection);
        GroundShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, groundPos);
        shader.setMat4("model", model);
        //Drawing the curved ground
        glBindVertexArray(curvedground.getVAO());
        glDrawElements(GL_TRIANGLES, curvedground.getIndexCount(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        //Draw a character object 
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, characterPos);
        model = glm::scale(model, glm::vec3(0.50f)); // a smaller cube
        shader.setMat4("model", model);
        character.drawCharacter();

        // Swap the screen buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window, CurvedGround& curvedground, glm::vec3& characterPos)
{
    // Define boundaries of the curved ground
    float minX = 0.0;
    float maxX = 8.0;
    float minZ = 0.0;
    float maxZ = 8.0;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    // Move character based on WASD keys
    const float characterSpeed = 2.5f * deltaTime; // Adjust speed as needed
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        glm::vec3 newPosition = characterPos + camera.Front * characterSpeed;
        // Check for collisions with boxes
        bool collisionDetected = false;
        for (const auto& box : boxes) {
            if (detectCollision(newPosition, box.getMin(), box.getMax())) {
                // Collision detected with this box, don't allow movement
                collisionDetected = true;
                break;
            }
        }
        // If no collision detected and within boundaries, update character position
        if (!collisionDetected && newPosition.x >= minX && newPosition.x <= maxX && newPosition.z >= minZ && newPosition.z <= maxZ)
            characterPos = newPosition;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        glm::vec3 newPosition = characterPos - camera.Front * characterSpeed;
        // Check for collisions with boxes
        bool collisionDetected = false;
        for (const auto& box : boxes) {
            if (detectCollision(newPosition, box.getMin(), box.getMax())) {
                // Collision detected with this box, don't allow movement
                collisionDetected = true;
                break;
            }
        }
        // If no collision detected and within boundaries, update character position
        if (!collisionDetected && newPosition.x >= minX && newPosition.x <= maxX && newPosition.z >= minZ && newPosition.z <= maxZ)
            characterPos = newPosition;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 newPosition = characterPos - glm::normalize(glm::cross(camera.Front, camera.Up)) * characterSpeed;
        // Check for collisions with boxes
        bool collisionDetected = false;
        for (const auto& box : boxes) {
            if (detectCollision(newPosition, box.getMin(), box.getMax())) {
                // Collision detected with this box, don't allow movement
                collisionDetected = true;
                break;
            }
        }
        // If no collision detected and within boundaries, update character position
        if (!collisionDetected && newPosition.x >= minX && newPosition.x <= maxX && newPosition.z >= minZ && newPosition.z <= maxZ)
            characterPos = newPosition;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 newPosition = characterPos + glm::normalize(glm::cross(camera.Front, camera.Up)) * characterSpeed;
        // Check for collisions with boxes
        bool collisionDetected = false;
        for (const auto& box : boxes) {
            if (detectCollision(newPosition, box.getMin(), box.getMax())) {
                // Collision detected with this box, don't allow movement
                collisionDetected = true;
                break;
            }
        }
        // If no collision detected and within boundaries, update character position
        if (!collisionDetected && newPosition.x >= minX && newPosition.x <= maxX && newPosition.z >= minZ && newPosition.z <= maxZ)
            characterPos = newPosition;
    }

    // Adjust character's height based on ground elevation
    float groundHeight = interpolateHeight(characterPos, curvedground);
    characterPos.y = groundHeight;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

glm::vec3 calculateBarycentricCoordinates(const glm::vec3& point, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) 
{
    glm::vec3 v0v1 = v1 - v0;
    glm::vec3 v0v2 = v2 - v0;
    glm::vec3 v0p = point - v0;

    float d00 = glm::dot(v0v1, v0v1);
    float d01 = glm::dot(v0v1, v0v2);
    float d11 = glm::dot(v0v2, v0v2);
    float d20 = glm::dot(v0p, v0v1);
    float d21 = glm::dot(v0p, v0v2);
    float denom = d00 * d11 - d01 * d01;

    glm::vec3 barycentric;
    barycentric.y = (d11 * d20 - d01 * d21) / denom;
    barycentric.z = (d00 * d21 - d01 * d20) / denom;
    barycentric.x = 1.0f - barycentric.y - barycentric.z;

    return barycentric;
}

float interpolateHeight(const glm::vec3& characterPos, CurvedGround& ground) 
{
    // Get the vertices and indices of the curved ground
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    ground.readDataFromFile("data.txt", vertices, indices);

    // Iterate through each triangle in the ground
    for (size_t i = 0; i < indices.size(); i += 3) {
        // Get the vertices of the current triangle
        glm::vec3 v0(vertices[indices[i] * 6], vertices[indices[i] * 6 + 1], vertices[indices[i] * 6 + 2]);
        glm::vec3 v1(vertices[indices[i + 1] * 6], vertices[indices[i + 1] * 6 + 1], vertices[indices[i + 1] * 6 + 2]);
        glm::vec3 v2(vertices[indices[i + 2] * 6], vertices[indices[i + 2] * 6 + 1], vertices[indices[i + 2] * 6 + 2]);

        // Calculate barycentric coordinates
        glm::vec3 barycentric = calculateBarycentricCoordinates(characterPos, v0, v1, v2);

        // Check if the character is inside the current triangle
        if (barycentric.x >= 0 && barycentric.y >= 0 && barycentric.z >= 0) {
            // Interpolate height using barycentric coordinates
            return barycentric.x * v0.y + barycentric.y * v1.y + barycentric.z * v2.y;
        }
    }


    // Default height if no triangle is found
    return 0.25f;
}

glm::vec3 calculateCharacterGroundPosition(const glm::vec3& characterPos, CurvedGround& ground) 
{
    // Get the vertices and indices of the curved ground
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    ground.readDataFromFile("data.txt", vertices, indices);

    // Iterate through each triangle in the ground
    for (size_t i = 0; i < indices.size(); i += 3) {
        // Get the vertices of the current triangle
        glm::vec3 v0(vertices[indices[i] * 6], vertices[indices[i] * 6 + 1], vertices[indices[i] * 6 + 2]);
        glm::vec3 v1(vertices[indices[i + 1] * 6], vertices[indices[i + 1] * 6 + 1], vertices[indices[i + 1] * 6 + 2]);
        glm::vec3 v2(vertices[indices[i + 2] * 6], vertices[indices[i + 2] * 6 + 1], vertices[indices[i + 2] * 6 + 2]);

        // Calculate barycentric coordinates
        glm::vec3 barycentric = calculateBarycentricCoordinates(characterPos, v0, v1, v2);

        // Check if the character is inside the current triangle
        if (barycentric.x >= 0 && barycentric.y >= 0 && barycentric.z >= 0) {
            // Interpolate height using barycentric coordinates
            float interpolatedHeight = barycentric.x * v0.y + barycentric.y * v1.y + barycentric.z * v2.y;
            return glm::vec3(characterPos.x, interpolatedHeight, characterPos.z);
        }
    }

    // Default position if no triangle is found
    return glm::vec3(characterPos.x, 0.25f, characterPos.z);
}

bool detectCollision(const glm::vec3& characterPos, const glm::vec3& boxMin, const glm::vec3& boxMax) {
    // Check if character's position is within the bounding box defined by boxMin and boxMax
    return (characterPos.x >= boxMin.x && characterPos.x <= boxMax.x &&
        characterPos.y >= boxMin.y && characterPos.y <= boxMax.y &&
        characterPos.z >= boxMin.z && characterPos.z <= boxMax.z);
}