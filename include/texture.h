#pragma once

#include <string>
#include <SDL3/SDL.h>
// #include "app.h"

class App;

class Texture {
public:
  Texture();
  ~Texture();

  bool loadFromFile(std::string path, App &app);
  void render(float x, float y, App &app);
  void destroy();

  int getWidth();
  int getHeight();

private:
  SDL_Texture *_texture;
  int _width;
  int _height;
};

