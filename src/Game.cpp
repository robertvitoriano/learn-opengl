#include <Game.hpp>

void Game::loop()
{
  int FPS = 60;
  Uint32 FRAME_UPDATE_TIME = 1000 / FPS;
  Uint32 mainTimer = 0;
  
  this->createWindow();
  
  Shader shader("../shaders/vertex_shader.glsl", "../shaders/fragment_shader.glsl");
  
  Cube cube(&shader);
  
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
          cube.setXOffset(cube.getXOffset() + 0.1);
        }
        else if (event.key.keysym.sym == SDLK_LEFT)
        {
          cube.setXOffset(cube.getXOffset() - 0.1);
        }
        else if (event.key.keysym.sym == SDLK_UP)
        {
          cube.setYOffset(cube.getYOffset() + 0.1);
        }
        else if (event.key.keysym.sym == SDLK_DOWN)
        {
          cube.setYOffset(cube.getYOffset() - 0.1);
        }
        else if (event.key.keysym.sym == SDLK_MINUS)
        {
          cube.setScale(cube.getScale() - 0.1);
          if (cube.getScale() < 0.1f)
            cube.setScale(0.1f);
        }
        else if (event.key.keysym.sym == SDLK_EQUALS)
        {
          if (event.key.keysym.mod & KMOD_SHIFT)
          {
            cube.setScale(cube.getScale() + 0.1);
          }
        }
        
      bool shiftPressed = (event.key.keysym.mod & KMOD_LSHIFT) || (event.key.keysym.mod & KMOD_RSHIFT);
      
       if (shiftPressed) {
        if (event.key.keysym.sym == SDLK_x) {
           cube.setRotationAngleX(cube.getRotationAngleX() + 10.0f);
        }
        if (event.key.keysym.sym == SDLK_y) {
           cube.setRotationAngleY(cube.getRotationAngleY() + 10.0f);
        }
        if (event.key.keysym.sym == SDLK_z) {
           cube.setRotationAngleZ(cube.getRotationAngleZ() + 10.0f);
        }
    }
      }
    }
    cube.draw();

    SDL_GL_SwapWindow(window);

    if (SDL_GetTicks() + FRAME_UPDATE_TIME >= mainTimer)
    {
      mainTimer = SDL_GetTicks() + FRAME_UPDATE_TIME;
    }
  }

  SDL_GL_DeleteContext(this->glContext);
  SDL_DestroyWindow(this->window);
  SDL_Quit();

}
 
void Game::createWindow(){
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cerr << "SDL Initialization Error: " << SDL_GetError() << std::endl;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  this->window = SDL_CreateWindow(
      "OpenGL Window",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      800, 600,
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (!this->window)
  {
    std::cerr << "Window Creation Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
  }

  this->glContext = SDL_GL_CreateContext(this->window);
  if (!this->glContext)
  {
    std::cerr << "OpenGL Context Creation Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(this->window);
    SDL_Quit();
  }

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    SDL_GL_DeleteContext(this->glContext);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
  }

  glViewport(0, 0, 800, 600);
  glEnable(GL_DEPTH_TEST);
}
