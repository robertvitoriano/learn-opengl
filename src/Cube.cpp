#include <iostream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <Cube.hpp>

Cube::Cube(Shader *shader, std::string texturePath)
    : transformationMatrix(glm::mat4(1.0f)),
      colorR(0.0f), colorG(0.0f), colorB(0.0f),
      scale(1.0f), xOffset(0.0f), yOffset(0.0f),
      rotationAngleX(0.0f), rotationAngleY(0.0f), rotationAngleZ(0.0f),
      currentShader(shader)
{
  this->initializeGraphicsPipeline();
  this->loadTexture(texturePath);
}

void Cube::initializeGraphicsPipeline()
{
  float vertices[] = {
      // FIRST FACE (Front)
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // Top Right
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // Bottom Right
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // Bottom Left
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // Top Left

      // SECOND FACE (Back)
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // Top Right
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // Bottom Right
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom Left
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // Top Left

      // THIRD FACE (Left)
      -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // Top Right
      -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // Bottom Right
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // Bottom Left
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // Top Left

      // FOURTH FACE (Right)
      0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // Top Right
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // Bottom Right
      0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Bottom Left
      0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // Top Left

      // FIFTH FACE (Top)
      0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // Top Right
      -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, // Bottom Right
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,  // Bottom Left
      0.5f, 0.5f, 0.5f, 0.0f, 1.0f,   // Top Left

      // SIXTH FACE (Bottom)
      0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // Top Right
      -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // Bottom Right
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  // Bottom Left
      0.5f, -0.5f, 0.5f, 0.0f, 1.0f    // Top Left
  };

  unsigned int indices[] = {
      // FIRST FACE (Front)
      0, 1, 3,
      1, 2, 3,

      // SECOND FACE (Back)
      4, 5, 7,
      5, 6, 7,

      // THIRD FACE (Left)
      8, 9, 11,
      9, 10, 11,

      // FOURTH FACE (Right)
      12, 13, 15,
      13, 14, 15,

      // FIFTH FACE (Top)
      16, 17, 19,
      17, 18, 19,

      // SIXTH FACE (Bottom)
      20, 21, 23,
      21, 22, 23};

  unsigned int vertexBufferObject, elementBufferObject;
  glGenVertexArrays(1, &vertexArrayObject);
  glGenBuffers(1, &vertexBufferObject);
  glGenBuffers(1, &elementBufferObject);

  glBindVertexArray(vertexArrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  int vertexStride = 5;

  int positionAttributeIndex = 0;
  int positionAtribbuteLength = 3;
  int positionAttributeOffset = 0;

  glVertexAttribPointer(positionAttributeIndex, positionAtribbuteLength, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void *)positionAttributeOffset);
  glEnableVertexAttribArray(positionAttributeIndex);

  int textureAttributeIndex = 1;
  int textureAtribbuteLength = 2;
  int textureAttributeOffset = 3;

  glVertexAttribPointer(textureAttributeIndex, textureAtribbuteLength, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void *)(textureAttributeOffset * sizeof(float)));
  glEnableVertexAttribArray(textureAttributeIndex);

  glBindVertexArray(0);
}

void Cube::draw(Shader *shader)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  shader->use();

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); 
  
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(xOffset, yOffset, 0.0f));
  model = glm::rotate(model, glm::radians(rotationAngleX), glm::vec3(1.0, 0.0, 0.0));
  model = glm::rotate(model, glm::radians(rotationAngleY), glm::vec3(0.0, 1.0, 0.0));
  model = glm::rotate(model, glm::radians(rotationAngleZ), glm::vec3(1.0, 0.0, 1.0));
  model = glm::scale(model, glm::vec3(scale, scale, scale));

  shader->setMat4F("view", view);
  shader->setMat4F("projection", projection);
  shader->setMat4F("model", model);
  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glBindVertexArray(vertexArrayObject);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Cube::loadTexture(std::string imagePath)
{

  unsigned int texture;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;

  unsigned char *data = stbi_load(imagePath.c_str(), &width, &height, &nrChannels, 0);

  if (data)
  {
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
      std::cerr << "OpenGL Error: " << error << std::endl;
    }
  }

  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  glBindTexture(GL_TEXTURE_2D, texture);
}

void Cube::setScale(float newScale) { scale = newScale; }
void Cube::setXOffset(float offset) { xOffset = offset; }
void Cube::setYOffset(float offset) { yOffset = offset; }
void Cube::setRotationAngleX(float angle) { rotationAngleX = angle; }
void Cube::setRotationAngleY(float angle) { rotationAngleY = angle; }
void Cube::setRotationAngleZ(float angle) { rotationAngleZ = angle; }
void Cube::setTransformationMatrix(const glm::mat4 &matrix) { transformationMatrix = matrix; }

float Cube::getScale()
{
  return scale;
}

float Cube::getXOffset()
{
  return xOffset;
}

float Cube::getYOffset()
{
  return yOffset;
}

float Cube::getRotationAngleX()
{
  return rotationAngleX;
}

float Cube::getRotationAngleY()
{
  return rotationAngleY;
}

float Cube::getRotationAngleZ()
{
  return rotationAngleZ;
}

glm::mat4 Cube::getTransformationMatrix() const { return transformationMatrix; }
