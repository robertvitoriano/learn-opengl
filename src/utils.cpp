#include "utils.h"

const char *readFile(const std::string &filePath)
{
  std::ifstream file(filePath, std::ios::in | std::ios::binary);
  if (!file)
  {
    std::cerr << "Could not open the file: " << filePath << std::endl;
    return nullptr;
  }
  std::string content;

  file.seekg(0, std::ios::end);

  content.reserve(file.tellg());

  file.seekg(0, std::ios::beg);

  content.assign((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

  char *result = new char[content.size() + 1];
  std::copy(content.c_str(), content.c_str() + content.size() + 1, result);

  return result;
}
