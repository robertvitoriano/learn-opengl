#include <main.h>

GLfloat colorR = 0.0f;
GLfloat colorG = 0.0f;
GLfloat colorB = 0.0f;

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
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      update();
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

unsigned int compileShader(unsigned int shaderType, const char *shaderSource)
{
  unsigned int fragmentShader;

  fragmentShader = glCreateShader(shaderType);
  glShaderSource(fragmentShader, 1, &shaderSource, NULL);
  glCompileShader(fragmentShader);
  int success;
  char infoLog[512];

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  return fragmentShader;
}

unsigned int getShaderProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  int success;
  char infoLog[512];

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  return shaderProgram;
}

void update()
{
  const char *vertexShaderSource = readFile("../shaders/vertex_shader.glsl");
  const char *fragmentShaderSource = readFile("../shaders/fragment_shader.glsl");
  unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
  unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  unsigned int shaderProgram = getShaderProgram(vertexShader, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float vertices[] = {
      0.5f, 1.0f, 0.0f,  // top right
      0.5f, 0.0f, 0.0f,  // bottom right
      -0.5f, 0.0f, 0.0f, // bottom left
      -0.5f, 1.0f, 0.0f, // top left
      0.0f, -0.5f, 0.0f, // 4
      0.0f, 0.0f, 0.0f,  // 5
  };
  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3, // second triangle
      1, 4, 5, // bottom triangle
      4, 2, 5};
  unsigned int elementBufferObject;
  glGenBuffers(1, &elementBufferObject);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  unsigned int vertexArrayObject, vertexBufferObject;

  glGenVertexArrays(1, &vertexArrayObject);
  glGenBuffers(1, &vertexBufferObject);

  glBindVertexArray(vertexArrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  glUseProgram(shaderProgram);

  glBindVertexArray(vertexArrayObject);

  GLint colorLocation = glGetUniformLocation(shaderProgram, "ourColor");

  glUniform3f(colorLocation, 1.0f, 0.0f, 0.0f);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

  glUniform3f(colorLocation, 0.0f, 0.0f, 1.0f);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(3 * sizeof(unsigned int)));

  GLfloat timeValue = SDL_GetTicks() / 1000.0f;
  GLfloat redValue = (sin(timeValue) / 4.0f) + 0.5f;
  GLfloat green = (sin(timeValue) / 4.0f) + 0.5f;
  GLfloat blue = (sin(timeValue) / 4.0f) + 0.5f;

  glUniform3f(colorLocation, redValue, green, blue);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(6 * sizeof(unsigned int)));

  float greenValue = sin(timeValue) / 2.0f + 0.5f;
  glUniform3f(colorLocation, 0.0f, greenValue, 0.0f);

  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void *)(9 * sizeof(unsigned int)));

  glBindVertexArray(0);
}
