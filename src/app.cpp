#include "app.h"
#include "assetManager.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <memory>
#include <spdlog/spdlog.h>

App::App() : _assetManager(std::make_unique<AssetManager>()), _textures{} {
  spdlog::debug("Creating app");
  this->_renderer = nullptr;
  this->_window = nullptr;
  this->_isRunning = false;
  this->_currentTexture = "UP";
}

App::~App() {
  spdlog::debug("Cleaning textures");

  spdlog::info("Quitting");
  SDL_Quit();
}

bool App::init() {
  spdlog::info("Initializing App");
  bool success = true;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    spdlog::error("SDL could not initialize! SDL error: {}", SDL_GetError());
    success = false;
  } else {
    SDL_Window *windowPtr = nullptr;
    SDL_Renderer *rendererPtr = nullptr;
    if (!SDL_CreateWindowAndRenderer("Textures", 800, 400, SDL_WINDOW_RESIZABLE, &windowPtr, &rendererPtr)) {
      spdlog::error("Window could not be created! SDL error: {}", SDL_GetError());
      success = false;
    } 
    _window.reset(windowPtr);
    _renderer.reset(rendererPtr);
    spdlog::debug("Initialised App window and renderer");

    _textures["UP"] = _assetManager->loadTexture("UP", "girl.png", _renderer.get());
    _textures["DOWN"] = _assetManager->loadTexture("DOWN", "emoticons.png", _renderer.get());
    _textures["RIGHT"] = _assetManager->loadTexture("RIGHT", "honey.png", _renderer.get());
    _textures["LEFT"] = _assetManager->loadTexture("LEFT", "penguin.png", _renderer.get());

    for (const auto &[key, texture]: _textures) {
      if (!texture) {
        success = false;
        spdlog::error("Failed to load texture {}", texture->getPath());
      }
    }
  }
  return success;
}

void App::run() {
  SDL_Event event;
  _isRunning = true;
  while (_isRunning) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        _isRunning = false;
      } else if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
          _isRunning = false;
        }
        if (event.key.key == SDLK_UP) {
          _currentTexture = "UP";
        } else if (event.key.key == SDLK_DOWN) {
          _currentTexture = "DOWN";
        } else if (event.key.key == SDLK_LEFT) {
          _currentTexture = "LEFT";
        } else if (event.key.key == SDLK_RIGHT) {
          _currentTexture = "RIGHT";
        }
      }
    }
    SDL_SetRenderDrawColor(_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_renderer.get());
    auto it = _textures.find(_currentTexture);
    if (it == _textures.end()) {
      spdlog::error("Cannot find {} in _textures", _currentTexture);
      continue;
    }
    int w = 0;
    int h = 0;
    SDL_GetWindowSize(_window.get(), &w, &h);
    SDL_RenderTexture(_renderer.get(), it->second->getTexture().get(), NULL, NULL);
    SDL_RenderPresent(_renderer.get());
  }
}

SDL_Window *App::getWindow() const {
  return this->_window.get();
}

SDL_Renderer *App::getRenderer() const {
  return this->_renderer.get();
}

std::map<std::string, std::shared_ptr<Texture>> App::getTextures() const {
  return this->_textures;
}

bool App::isRunning() const {
  return this->_isRunning;
}
