#ifndef MAIN_H
#define Main_H
#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <random>
#include <utils.h>
#include <shader.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Cube.hpp>
void initializeGraphicsPipeline();
void draw(Shader *shader);
SDL_Window *createWindow();
SDL_GLContext setupOpenGLContext(SDL_Window *window);
void loadTexture(std::string imagePath);
#endif
