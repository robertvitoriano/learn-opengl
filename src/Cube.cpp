#include <iostream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <Cube.hpp>


Cube::Cube(Shader *shader)
    : transformationMatrix(glm::mat4(1.0f)),
      colorR(0.0f), colorG(0.0f), colorB(0.0f),
      scale(1.0f), xOffset(0.0f), yOffset(0.0f),
      rotationAngleX(0.0f), rotationAngleY(0.0f), rotationAngleZ(0.0f),
      currentShader(shader)
{
  initializeGraphicsPipeline();
}

void Cube::initializeGraphicsPipeline()
{
  float vertices[] = {
      // Define vertices for cube faces with color and texture info
      // (values omitted for brevity)
  };

  unsigned int indices[] = {
      // Indices for cube faces (values omitted for brevity)
  };

  glGenVertexArrays(1, &vertexArrayObject);
  glGenBuffers(1, &vertexBufferObject);
  glGenBuffers(1, &elementBufferObject);

  glBindVertexArray(vertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  int vertexStride = 5;
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertexStride * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void Cube::draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  currentShader->use();

  transformationMatrix = glm::scale(transformationMatrix, glm::vec3(scale, scale, scale));
  transformationMatrix = glm::rotate(transformationMatrix, glm::radians(rotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
  transformationMatrix = glm::rotate(transformationMatrix, glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
  transformationMatrix = glm::rotate(transformationMatrix, glm::radians(rotationAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));
  transformationMatrix = glm::translate(transformationMatrix, glm::vec3(-0.5f + xOffset, yOffset, 0.0f));

  currentShader->setMat4F("transform", transformationMatrix);

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
}

std::string Cube::mat4ToString(const glm::mat4 &matrix) const
{
  std::stringstream ss;
  for (int i = 0; i < 4; ++i)
  {
    ss << "[ ";
    for (int j = 0; j < 4; ++j)
    {
      ss << matrix[i][j] << " ";
    }
    ss << "]\n";
  }
  return ss.str();
}

void Cube::printTransformationMatrix() const
{
  std::cout << mat4ToString(transformationMatrix) << std::endl;
}

// Setters
void Cube::setColor(float r, float g, float b)
{
  colorR = r;
  colorG = g;
  colorB = b;
}
void Cube::setScale(float newScale) { scale = newScale; }
void Cube::setXOffset(float offset) { xOffset = offset; }
void Cube::setYOffset(float offset) { yOffset = offset; }
void Cube::setRotationAngleX(float angle) { rotationAngleX = angle; }
void Cube::setRotationAngleY(float angle) { rotationAngleY = angle; }
void Cube::setRotationAngleZ(float angle) { rotationAngleZ = angle; }
void Cube::setTransformationMatrix(const glm::mat4 &matrix) { transformationMatrix = matrix; }

glm::mat4 Cube::getTransformationMatrix() const { return transformationMatrix; }
