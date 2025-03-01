#include "app.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>


App::App() {
  SDL_Log("Creating app\n");
  this->_renderer = nullptr;
  this->_window = nullptr;
  this->_texture = nullptr;
  this->_isRunning = false;
}

App::~App() {
  SDL_Log("Destroying window\n");
  SDL_DestroyWindow(_window);
  SDL_Log("Destroying renderer\n");
  SDL_DestroyRenderer(_renderer);
  SDL_Log("Destroying texture\n");
  this->_texture->destroy();

  SDL_Log("Quitting SDL\n");
  SDL_Quit();
}

bool App::init() {
  bool success = true;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
    success = false;
  } else {
    if (!SDL_CreateWindowAndRenderer("Textures", 800, 400, 0, &_window, &_renderer)) {
      SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
      success = false;
    } 
    // if (success = this->_texture->loadFromFile("assets/test.png"), !success) {
    //   SDL_Log("Unable to load png image!\n");
    // }
  }
  return success;
}

void App::run() {
  SDL_Event event;
  this->_isRunning = true;
  while (_isRunning) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          _isRunning = false ;
        default: break ;
      }
    }
  }
}

SDL_Window *App::getWindow() const {
  return this->_window;
}

SDL_Renderer *App::getRenderer() const {
  return this->_renderer;
}

Texture *App::getTexture() const {
  return this->_texture;
}

bool App::isRunning() const {
  return this->_isRunning;
}
