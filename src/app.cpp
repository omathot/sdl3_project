#include "app.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

App::App() : _textures{} {
  SDL_Log("Creating app\n");
  this->_renderer = nullptr;
  this->_window = nullptr;
  this->_isRunning = false;
  this->_currentTexture = "";
}

App::~App() {
  SDL_Log("Destroying window\n");
  SDL_DestroyWindow(_window);
  SDL_Log("Destroying renderer\n");
  SDL_DestroyRenderer(_renderer);
  SDL_Log("Destroying textures\n");

  SDL_Log("Quitting SDL\n");
  SDL_Quit();
}

bool App::init() {
  SDL_Log("Initializing\n");
  bool success = true;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
    success = false;
  } else {
    if (!SDL_CreateWindowAndRenderer("Textures", 800, 400, SDL_WINDOW_RESIZABLE, &_window, &_renderer)) {
      SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
      success = false;
    } 
    success &= this->_textures["UP"].loadFromFile("C:/Users/Oscar/dev/projects/build/sdl3_project/assets/emoticons.png", this);
    success &= this->_textures["DOWN"].loadFromFile("C:/Users/Oscar/dev/projects/build/sdl3_project/assets/girl.png", this);
    success &= this->_textures["RIGHT"].loadFromFile("C:/Users/Oscar/dev/projects/build/sdl3_project/assets/honey.png", this);
    success &= this->_textures["LEFT"].loadFromFile("C:/Users/Oscar/dev/projects/build/sdl3_project/assets/penguin.png", this);
    if (!success) {
      SDL_Log("Failed to load images !\n");
    }
  }
  std::cout << "returning success: " << success << std::endl;
  return success;
}

void App::run() {
  SDL_Event event;
  this->_isRunning = true;
  while (_isRunning) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        _isRunning = false;
      } else if (event.type == SDL_EVENT_KEY_DOWN) {
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
    SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(_renderer);
    for (auto &texture: _textures) {
      if (_currentTexture == texture.first) {
        texture.second.render(0.0f, 0.0f, this);
      }
    }
    SDL_RenderPresent(_renderer);
  }
}

SDL_Window *App::getWindow() const {
  return this->_window;
}

SDL_Renderer *App::getRenderer() const {
  return this->_renderer;
}

std::map<std::string, Texture> App::getTextures() const {
  return this->_textures;
}

bool App::isRunning() const {
  return this->_isRunning;
}
