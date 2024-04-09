#include "CurvedGround.h"
#include <fstream>
#include <sstream>

CurvedGround::CurvedGround()
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
    indexCount = 0;
}

CurvedGround::~CurvedGround()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void CurvedGround::loadCurvedGround(const char* filename)
{
    vector<GLfloat> groundVertices;
    vector<GLuint> groundIndices;
    readDataFromFile(filename, groundVertices, groundIndices);


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, groundVertices.size() * sizeof(GLfloat), groundVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, groundIndices.size() * sizeof(GLuint), groundIndices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    indexCount = groundIndices.size();

    glBindVertexArray(0);
}

void CurvedGround::readDataFromFile(const char* filename, vector<GLfloat>& vertices, vector<GLuint>& indices)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    bool readingVertices = true;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        if (line == "// Indices") {
            readingVertices = false;
            continue;
        }

        std::istringstream iss(line);
        if (readingVertices) {
            GLfloat x, y, z, r, g, b;
            iss >> x >> y >> z >> r >> g >> b;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(r);
            vertices.push_back(g);
            vertices.push_back(b);
        }
        else {
            GLuint i1, i2, i3;
            iss >> i1 >> i2 >> i3;
            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        }
    }

    file.close();
}

GLuint CurvedGround::getVAO() const
{
    return VAO;
}

GLuint CurvedGround::getVBO() const
{
    return VBO;
}

GLuint CurvedGround::getEBO() const
{
    return EBO;
}

GLsizei CurvedGround::getIndexCount() const
{
    return indexCount;
}