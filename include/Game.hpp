#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <Shader.hpp>
#include <Cube.hpp>
#include <glm/glm.hpp>


class Game {
public:
    void loop();
private:
    void createWindow();
    SDL_GLContext glContext;
    SDL_Window* window;
};

#endif
