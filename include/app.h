#pragma once
#include "texture.h"
#include <SDL3/SDL.h>

class App {
public:
  App();
  ~App();

  bool init();
  void run();
  
  SDL_Renderer *getRenderer() const;
  SDL_Window *getWindow() const;
  Texture *getTexture() const;
  bool isRunning() const;

private:
  SDL_Window *_window;
  SDL_Renderer *_renderer;
  Texture *_texture;
  bool _isRunning;
};
