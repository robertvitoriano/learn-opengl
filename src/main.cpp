#include <main.h>

GLfloat colorR = 0.0f;
GLfloat colorG = 0.0f;
GLfloat colorB = 0.0f;

unsigned int vertexArrayObject;

float scale = 0.5f;
float xOffset = 0.0f;
float yOffset = 0.0f;

float rotationAngleX = 0.0f;
float rotationAngleY = 0.0f;
float rotationAngleZ = 0.0f;

int main(int argc, char *argv[])
{
  int FPS = 60;
  Uint32 FRAME_UPDATE_TIME = 1000 / FPS;
  Uint32 mainTimer = 0;

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cerr << "SDL Initialization Error: " << SDL_GetError() << std::endl;
    return -1;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_Window *window = SDL_CreateWindow(
      "OpenGL Window",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      800, 600,
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!window)
  {
    std::cerr << "Window Creation Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return -1;
  }

  SDL_GLContext glContext = SDL_GL_CreateContext(window);
  if (!glContext)
  {
    std::cerr << "OpenGL Context Creation Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glEnable(GL_DEPTH_TEST);

  Shader shader("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");

  initializeGraphicsPipeline(&shader);

  bool running = true;
  SDL_Event event;

  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = false;
      }

      if (event.type == SDL_KEYDOWN)
      {
        if (event.key.keysym.sym == SDLK_RIGHT)
        {
          xOffset += 0.1;
        }
        else if (event.key.keysym.sym == SDLK_LEFT)
        {
          xOffset -= 0.1;
        }
        else if (event.key.keysym.sym == SDLK_UP)
        {
          yOffset += 0.1;
        }
        else if (event.key.keysym.sym == SDLK_DOWN)
        {
          yOffset -= 0.1;
        }
        else if (event.key.keysym.sym == SDLK_MINUS)
        {
          std::cout << "MINUS WAS PRESSED" << std::endl;
          scale -= 0.1f;
          if (scale < 0.1f)
            scale = 0.1f;
        }
        else if (event.key.keysym.sym == SDLK_EQUALS)
        {
          if (event.key.keysym.mod & KMOD_SHIFT)
          {
            std::cout << "PLUS WAS PRESSED" << std::endl;
            scale += 0.1f;
          }
        }
        
      bool shiftPressed = (event.key.keysym.mod & KMOD_LSHIFT) || (event.key.keysym.mod & KMOD_RSHIFT);
      
       if (shiftPressed) {
        if (event.key.keysym.sym == SDLK_x) {
            rotationAngleX += 10.0f;
        }
        if (event.key.keysym.sym == SDLK_y) {
            rotationAngleY += 10.0f;
        }
        if (event.key.keysym.sym == SDLK_z) {
            rotationAngleZ += 10.0f;
        }
    }
      }
    }
    draw(&shader);

    SDL_GL_SwapWindow(window);

    if (SDL_GetTicks() + FRAME_UPDATE_TIME >= mainTimer)
    {
      mainTimer = SDL_GetTicks() + FRAME_UPDATE_TIME;
    }
  }

  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

void initializeGraphicsPipeline(Shader *shader)
{
  float vertices[] = {
      // FIRST FACE (Front)
      0.5f, 0.5f, 0.5f,  1.0f, 1.0f,   // Top Right
      0.5f, -0.5f, 0.5f,  1.0f, 0.0f,  // Bottom Right
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // Bottom Left
      -0.5f, 0.5f, 0.5f,  0.0f, 1.0f,  // Top Left

      // SECOND FACE (Back)
      0.5f, 0.5f, -0.5f,  1.0f, 1.0f,   // Top Right
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  // Bottom Right
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom Left
      -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,  // Top Left

      // THIRD FACE (Left)
      -0.5f, 0.5f, -0.5f,  1.0f, 1.0f,  // Top Right
      -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom Right
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  // Bottom Left
      -0.5f, 0.5f, 0.5f,  0.0f, 1.0f,   // Top Left

      // FOURTH FACE (Right)
      0.5f, 0.5f, 0.5f,  1.0f, 1.0f,   // Top Right
      0.5f, -0.5f, 0.5f,  1.0f, 0.0f,  // Bottom Right
      0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom Left
      0.5f, 0.5f, -0.5f,  0.0f, 1.0f,  // Top Left

      // FIFTH FACE (Top)
      0.5f, 0.5f, -0.5f,  1.0f, 1.0f,  // Top Right
      -0.5f, 0.5f, -0.5f,  1.0f, 0.0f, // Bottom Right
      -0.5f, 0.5f, 0.5f,  0.0f, 0.0f,  // Bottom Left
      0.5f, 0.5f, 0.5f,  0.0f, 1.0f,   // Top Left

      // SIXTH FACE (Bottom)
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  // Top Right
      -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // Bottom Right
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,  // Bottom Left
      0.5f, -0.5f, 0.5f,  0.0f, 1.0f    // Top Left
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

  loadTexture("../container.jpg");

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

void draw(Shader *shader)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->use();

    // First Cube
    glm::mat4 transformationMatrix1 = glm::mat4(1.0f);
    transformationMatrix1 = glm::translate(transformationMatrix1, glm::vec3(-0.5f + xOffset, yOffset, 0.0f)); // Adjust position
    transformationMatrix1 = glm::rotate(transformationMatrix1, glm::radians(rotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
    transformationMatrix1 = glm::rotate(transformationMatrix1, glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
    transformationMatrix1 = glm::rotate(transformationMatrix1, glm::radians(rotationAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));
    transformationMatrix1 = glm::scale(transformationMatrix1, glm::vec3(scale, scale, scale));
    shader->setMat4F("transform", transformationMatrix1);

    glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // Second Cube
    glm::mat4 transformationMatrix2 = glm::mat4(1.0f);
    transformationMatrix2 = glm::translate(transformationMatrix2, glm::vec3(0.5f + xOffset, yOffset, 0.0f)); // Adjust position
    transformationMatrix2 = glm::rotate(transformationMatrix2, glm::radians(rotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
    transformationMatrix2 = glm::rotate(transformationMatrix2, glm::radians(rotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
    transformationMatrix2 = glm::rotate(transformationMatrix2, glm::radians(rotationAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));
    transformationMatrix2 = glm::scale(transformationMatrix2, glm::vec3(scale, scale, scale));
    shader->setMat4F("transform", transformationMatrix2);

    glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}


void loadTexture(std::string imagePath)
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

  glBindTexture(GL_TEXTURE_2D, texture);
}
