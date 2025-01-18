#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <utils.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
public:
  unsigned int ID;

  Shader(const std::string &vertexPath, const std::string &fragmentPath);
  void use();

  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec3f(const std::string &name, float color[3]) const;
  void setMat4F(const std::string &name, glm::mat4 transformationMatrix)const;
};

#endif
