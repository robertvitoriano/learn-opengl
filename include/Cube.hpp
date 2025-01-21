#ifndef CUBE_HPP
#define CUBE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <stb_image.h>
#include "Shader.hpp"

class Cube {
public:
    Cube(Shader* shader);

    float getColorR() const;
    float getColorG() const;
    float getColorB() const;
    float getScale() const;
    float getXOffset() const;
    float getYOffset() const;
    float getRotationAngleX() const;
    float getRotationAngleY() const;
    float getRotationAngleZ() const;
    glm::mat4 getTransformationMatrix() const;

    void setColor(float r, float g, float b);
    void setScale(float newScale);
    void setXOffset(float offset);
    void setYOffset(float offset);
    void setRotationAngleX(float angle);
    void setRotationAngleY(float angle);
    void setRotationAngleZ(float angle);
    void setTransformationMatrix(const glm::mat4& matrix);

    void initializeGraphicsPipeline();
    void draw();
    void loadTexture(std::string imagePath);

private:
    glm::mat4 transformationMatrix;
    float colorR, colorG, colorB;
    float scale;
    float xOffset, yOffset;
    float rotationAngleX, rotationAngleY, rotationAngleZ;

    Shader* currentShader;

    unsigned int vertexArrayObject;
    unsigned int vertexBufferObject;
    unsigned int elementBufferObject;
};

#endif // CUBE_HPP
