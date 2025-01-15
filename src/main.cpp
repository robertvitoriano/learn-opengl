#include <main.h>

GLfloat colorR = 0.0f;
GLfloat colorG = 0.0f;
GLfloat colorB = 0.0f;

unsigned int vertexArrayObject;

float xOffset = 0.0;

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

  glad_glViewport(0, 0, 800, 600);

  bool running = true;

  SDL_Event event;

  Shader shader("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
  int width, height, nrChannels;
  unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

  setupShader(&shader);

  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        running = false;
      }

      if (event.window.event == SDL_WINDOWEVENT_RESIZED)
      {
        int width = event.window.data1;
        int height = event.window.data2;
        handleWindowResize(width, height);
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
      }

      draw(&shader);
    }

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

void handleWindowResize(int width, int height)
{

  glViewport(0, 0, width, height);
}

void setupShader(Shader *shader)
{

  float vertices[] = {
      // positions         // colors
      0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
      -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
      0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top
  };

  float texCoords[] = {
      0.5, 0.0, // bottom
      0.0, 0.5, // left corner
      1.0, 0.0  // right corner
  }

  unsigned int indices[] = {
      0,
      1,
      2,
  };
  unsigned int elementBufferObject;
  glGenBuffers(1, &elementBufferObject);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  unsigned int vertexBufferObject;

  glGenVertexArrays(1, &vertexArrayObject);
  glGenBuffers(1, &vertexBufferObject);

  glBindVertexArray(vertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  shader->use();
}

void draw(Shader *shader)
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindVertexArray(vertexArrayObject);
  shader->setFloat("xOffset", xOffset);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}
