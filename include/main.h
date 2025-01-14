#ifndef MAIN_H
#define Main_H
#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <random>
#include <utils.h>
#include <shader.h>
void handleWindowResize(int width, int height);

void setupShader(Shader *shader);
void draw(Shader *shader);

#endif
