#include "app.h"
#include "assetManager.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <memory>
#include <spdlog/spdlog.h>

App::App() : _assetManager(std::make_unique<AssetManager>()), _textures{} {
  spdlog::debug("Creating app");
  this->_renderer = nullptr;
  this->_window = nullptr;
  this->_isRunning = false;
  this->_currentTexture = "UP";
  this->_animation = nullptr;
}

App::~App() {
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
    _assetManager->setRenderer(_renderer);

    _textures["UP"] = _assetManager->loadTexture("UP", "girl.png");
    _textures["DOWN"] = _assetManager->loadTexture("DOWN", "emoticons.png");
    _textures["RIGHT"] = _assetManager->loadTexture("RIGHT", "honey.png");
    _textures["LEFT"] = _assetManager->loadTexture("LEFT", "penguin.png");

    _animation = _assetManager->loadAnimation("player_idle", "V1/Player Idle/Player Idle 48x48.png", 48, 48, 10, 0.1);
    if (!_animation->isLoaded()) {
      success = false;
      spdlog::error("Failed to load animation {}", _animation->getPath());
    }
    spdlog::debug("Animation loaded at path {}", _animation->getPath());
    _animation->stop();

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
  bool showAnimation = false;

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
          showAnimation = false;
        } else if (event.key.key == SDLK_DOWN) {
          _currentTexture = "DOWN";
          showAnimation = false;
        } else if (event.key.key == SDLK_LEFT) {
          _currentTexture = "LEFT";
          showAnimation = false;
        } else if (event.key.key == SDLK_RIGHT) {
          _currentTexture = "RIGHT";
          showAnimation = false;
        } else if (event.key.key == SDLK_SPACE) {
          _currentTexture = "";
          showAnimation = true;
          _animation->play();
        }
      }
    }
    SDL_SetRenderDrawColor(_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_renderer.get());
    int w = 0;
    int h = 0;
    SDL_GetWindowSize(_window.get(), &w, &h);
    if (!showAnimation && _currentTexture != "") {
      auto it = _textures.find(_currentTexture);
      if (it == _textures.end()) {
        spdlog::error("Cannot find {} in _textures", _currentTexture);
        continue;
      }
      SDL_RenderTexture(_renderer.get(), it->second->getTexture().get(), NULL, NULL);
    } else {
      _animation->update();
      float x = (w - _animation->getFrameWidth()) / 2.0f;
      float y = (h - _animation->getFrameHeight()) / 2.0f;
      _animation->render(_renderer.get(), x, y, 0, 5);
    }
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

AssetManager *App::getAssetManager() const {
  return _assetManager.get();
}

bool App::isRunning() const {
  return this->_isRunning;
}
