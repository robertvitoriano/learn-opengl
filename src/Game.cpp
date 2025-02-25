#include <Game.hpp>

void Game::loop()
{
  int FPS = 60;
  Uint32 FRAME_UPDATE_TIME = 1000 / FPS;
  Uint32 mainTimer = 0;

  SDL_Window *window = this->createWindow();
  SDL_GLContext glContext = this->setupOpenGLContext(window);
  Shader shader("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
  Cube cube(&shader, "../agua.jpeg");
  this->player = &cube;

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

      this->handleInput(event);
    }
    this->player->draw(&shader);

    SDL_GL_SwapWindow(window);

    if (SDL_GetTicks() + FRAME_UPDATE_TIME >= mainTimer)
    {
      mainTimer = SDL_GetTicks() + FRAME_UPDATE_TIME;
    }
  }

  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
void Game::handleInput(SDL_Event event)
{
  if (event.type == SDL_KEYDOWN)
  {
    if (event.key.keysym.sym == SDLK_RIGHT)
    {
      this->player->setXOffset(this->player->getXOffset() + 0.1);
    }
    else if (event.key.keysym.sym == SDLK_LEFT)
    {
      this->player->setXOffset(this->player->getXOffset() - 0.1);
    }
    else if (event.key.keysym.sym == SDLK_UP)
    {
      this->player->setYOffset(this->player->getYOffset() + 0.1);
    }
    else if (event.key.keysym.sym == SDLK_DOWN)
    {
      this->player->setYOffset(this->player->getYOffset() - 0.1);
    }
    else if (event.key.keysym.sym == SDLK_d)
    {
      this->player->moveCameraRight();
    }
    else if (event.key.keysym.sym == SDLK_a)
    {
      this->player->moveCameraLeft();
    }
    else if (event.key.keysym.sym == SDLK_w)
    {
      this->player->moveCameraUp();
    }
    else if (event.key.keysym.sym == SDLK_s)
    {
      this->player->moveCameraDown();
    }
    else if (event.key.keysym.sym == SDLK_MINUS)
    {
      this->player->setScale(this->player->getScale() - 0.1);

      if (this->player->getScale() < 0.1f)
        this->player->setScale(0.1f);
    }
    else if (event.key.keysym.sym == SDLK_EQUALS)
    {
      if (event.key.keysym.mod & KMOD_SHIFT)
      {
        this->player->setScale(this->player->getScale() + 0.1);
      }
    }

    bool shiftPressed = (event.key.keysym.mod & KMOD_LSHIFT) || (event.key.keysym.mod & KMOD_RSHIFT);

    if (shiftPressed)
    {
      if (event.key.keysym.sym == SDLK_x)
      {
        this->player->setRotationAngleX(this->player->getRotationAngleX() + 10.0f);
      }
      if (event.key.keysym.sym == SDLK_y)
      {
        this->player->setRotationAngleY(this->player->getRotationAngleY() + 10.0f);
      }
      if (event.key.keysym.sym == SDLK_z)
      {
        this->player->setRotationAngleZ(this->player->getRotationAngleZ() + 10.0f);
      }
    }
  }
}
SDL_Window *Game::createWindow()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cerr << "SDL Initialization Error: " << SDL_GetError() << std::endl;
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
  }

  return window;
}

SDL_GLContext Game::setupOpenGLContext(SDL_Window *window)
{
  SDL_GLContext glContext = SDL_GL_CreateContext(window);
  if (!glContext)
  {
    std::cerr << "OpenGL Context Creation Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

  glViewport(0, 0, 800, 600);
  glEnable(GL_DEPTH_TEST);
  return glContext;
}
