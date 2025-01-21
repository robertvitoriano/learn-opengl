#include <Shader.hpp>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
{
  const char *vShaderCode = readFile(vertexPath);
  const char *fShaderCode = readFile(fragmentPath);

  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  };

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);

  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  };

  this->ID = glCreateProgram();
  glAttachShader(this->ID, vertex);
  glAttachShader(this->ID, fragment);
  glLinkProgram(this->ID);

  glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use()
{
  glUseProgram(this->ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
  glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setVec3f(const std::string &name, float color[3]) const
{
    GLint location = glGetUniformLocation(this->ID, name.c_str());

    glUniform3fv(location, 1, color);
}

void Shader::setMat4F(const std::string &name, glm::mat4 transformationMatrix)const{
  unsigned int transformLocation = glGetUniformLocation(this->ID, name.c_str());
  glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformationMatrix));
}
