#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include "shader.h"  

class Cube {
public:
    Cube(Shader* shader);
    void setColor(float r, float g, float b);
    void setScale(float newScale);
    void setXOffset(float offset);
    void setYOffset(float offset);
    void setRotationAngleX(float angle);
    void setRotationAngleY(float angle);
    void setRotationAngleZ(float angle);
    void setTransformationMatrix(const glm::mat4& matrix);
    glm::mat4 getTransformationMatrix() const;

    void printTransformationMatrix() const;

private:
    Shader* currentShader;
    unsigned int vertexArrayObject, vertexBufferObject, elementBufferObject;
    glm::mat4 transformationMatrix;

    float colorR, colorG, colorB;
    float scale, xOffset, yOffset;
    float rotationAngleX, rotationAngleY, rotationAngleZ;

    void initializeGraphicsPipeline();
    void loadTexture(std::string imagePath);
    void draw(Shader *shader);
    std::string mat4ToString(const glm::mat4& matrix) const;
};

#endif // CUBE_H
