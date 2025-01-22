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
    void setScale(float newScale);
    void setXOffset(float offset);
    void setYOffset(float offset);
    void setRotationAngleX(float angle);
    void setRotationAngleY(float angle);
    void setRotationAngleZ(float angle);
    
    float getColor();
    float getScale();
    float getXOffset();
    float getYOffset();
    float getRotationAngleX();
    float getRotationAngleY();
    float getRotationAngleZ();
    
    void setTransformationMatrix(const glm::mat4& matrix);
    glm::mat4 getTransformationMatrix() const;
    void draw(Shader *shader);
    void printTransformationMatrix() const;
    void initializeGraphicsPipeline();

private:
    Shader* currentShader;
    unsigned int vertexArrayObject, vertexBufferObject, elementBufferObject;
    glm::mat4 transformationMatrix;

    float colorR, colorG, colorB;
    float scale, xOffset, yOffset;
    float rotationAngleX, rotationAngleY, rotationAngleZ;
    void loadTexture(std::string imagePath);
    std::string mat4ToString(const glm::mat4& matrix) const;
};

#endif // CUBE_H
