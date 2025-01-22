#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <random>
#include <utils.h>
#include <shader.hpp>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Cube.hpp>
class Game {
  private:
    SDL_Window *createWindow();
    SDL_GLContext setupOpenGLContext(SDL_Window *window);
  public:
    void loop();
};
#endif
