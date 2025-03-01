#pragma once

#include "texture.h"
#include <SDL3/SDL.h>
#include <map>

class App {
public:
  App();
  ~App();

  bool init();
  void run();
  
  SDL_Renderer *getRenderer() const;
  SDL_Window *getWindow() const;
  std::map<std::string, Texture> getTextures() const;
  bool isRunning() const;

private:
  SDL_Window *_window;
  SDL_Renderer *_renderer;
  bool _isRunning;
  std::map<std::string, Texture> _textures;
};
