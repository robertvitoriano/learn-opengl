#include <main.h>

GLfloat colorR = 0.0f;
GLfloat colorG = 0.0f;
GLfloat colorB = 0.0f;

unsigned int vertexArrayObject;

float xOffset = 0.0f;
float yOffset = 0.0f;
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
    // Positions        // Colors       // Texture Coords
     0.5f,  0.5f, 0.0f,  0.5f, 0.1f, 0.4f,  1.0f, 1.0f,   // Top Right
     0.5f, -0.5f, 0.0f,  0.3f, 0.2f, 0.1f,  1.0f, 0.0f,   // Bottom Right
    -0.5f, -0.5f, 0.0f,  0.9f, 0.7f, 0.6f,  0.0f, 0.0f,   // Bottom Left
    -0.5f,  0.5f, 0.0f,  0.5f, 0.3f, 0.2f,  0.0f, 1.0f    // Top Left
};
  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3  // second triangle
  };

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;

  unsigned char *data = stbi_load("../container.jpg", &width, &height, &nrChannels, 0);
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

  unsigned int vertexBufferObject, elementBufferObject;
  glGenVertexArrays(1, &vertexArrayObject);
  glGenBuffers(1, &vertexBufferObject);
  glGenBuffers(1, &elementBufferObject);

  glBindVertexArray(vertexArrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  glBindTexture(GL_TEXTURE_2D, texture);
}

void draw(Shader *shader)
{
  glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
  glClear(GL_COLOR_BUFFER_BIT);

  shader->use();

  shader->setFloat("xOffset",xOffset);
  shader->setFloat("yOffset",yOffset);

  glBindVertexArray(vertexArrayObject);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
