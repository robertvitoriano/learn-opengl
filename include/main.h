#ifndef MAIN_H
#define Main_H
#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <random>

void handleWindowResize(int width, int height);
void update();
unsigned int getFragmentShader();
unsigned int getVertexShader();
unsigned int getShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);
#endif
