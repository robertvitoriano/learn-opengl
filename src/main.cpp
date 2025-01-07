#include <main.h>

GLfloat colorR = 0.0f;
GLfloat colorG = 0.0f;
GLfloat colorB = 0.0f;

const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);;\n"
                                   "}\0";

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

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

unsigned int getVertexShader()
{
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  return vertexShader;
}

unsigned int getFragmentShader()
{
  unsigned int fragmentShader;

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  int success;
  char infoLog[512];

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
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
  unsigned int vertexShader = getVertexShader();
  unsigned int fragmentShader = getFragmentShader();

  unsigned int shaderProgram = getShaderProgram(vertexShader, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float triangleVertices[] = {
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f, 0.5f, 0.0f};

  unsigned int vertexArrayObject, vertexBufferObject;

  glGenVertexArrays(1, &vertexArrayObject);
  glGenBuffers(1, &vertexBufferObject);

  glBindVertexArray(vertexArrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  glUseProgram(shaderProgram);
  glBindVertexArray(vertexArrayObject);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
