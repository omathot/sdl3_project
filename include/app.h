#pragma once

#include "texture.h"
#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>
#include <map>
#include <memory>
#include "animation.h"


class Player;

struct SDLWindowCleaner{
  void operator()(SDL_Window *window) {
    if (window) {
      spdlog::debug("Cleaning window");
      SDL_DestroyWindow(window);
    }
  }
};

struct SDLRendererCleaner {
  void operator()(SDL_Renderer *renderer) {
    if (renderer) {
      spdlog::debug("Cleaning renderer");
      SDL_DestroyRenderer(renderer);
    }
  }
};

class App {
public:
  App();
  ~App();

  bool init();
  void run();
  
  SDL_Renderer *getRenderer() const;
  SDL_Window *getWindow() const;
  std::map<std::string, std::shared_ptr<Texture>> getTextures() const;
  bool isRunning() const;

private:
  std::unique_ptr<SDL_Window, SDLWindowCleaner> _window;
  std::unique_ptr<SDL_Renderer, SDLRendererCleaner>_renderer;
  std::unique_ptr<AssetManager> _assetManager;
  bool _isRunning;
  std::map<std::string, std::shared_ptr<Texture>> _textures;
  std::string _currentTexture;

  // --
  // std::unique_ptr<Player> _player;
  std::shared_ptr<Animation> _animation;
};
