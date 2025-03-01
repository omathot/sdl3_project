#include "texture.h"
#include "app.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

Texture::Texture() {
  this->_texture = nullptr;
  this->_width = 0;
  this->_height = 0;
}
  
Texture::~Texture() {
  this->destroy();
}

bool Texture::loadFromFile(std::string path, App *app) {
  this->destroy();

  if (SDL_Surface *loadedSurface = IMG_Load(path.c_str()); loadedSurface == nullptr) {
    SDL_Log("Unable to load image %s! SDL_image error: %s\n", path.c_str(), SDL_GetError());
  } else {
      if (this->_texture = SDL_CreateTextureFromSurface(app->getRenderer(), loadedSurface), this->_texture == nullptr) {
      SDL_Log("Unable to create texture from loaded data! SDL error: %s\n", SDL_GetError());
    } else {
      this->_width = loadedSurface->w;
      this->_height = loadedSurface->h;
    }
    SDL_DestroySurface(loadedSurface);
  }
  return this->_texture != nullptr;
  
}

void Texture::destroy() {
  if (this->_texture != nullptr) {
    SDL_Log("Texture non null\n");
    SDL_DestroyTexture(this->_texture);
  }
  this->_texture = nullptr;
  this->_width = 0;
  this->_height = 0;
}

void Texture::render(float x, float y, App *app) {
  SDL_FRect dstRect = {x, y, static_cast<float>(this->_width), static_cast<float>(this->_height)};
  SDL_RenderTexture(app->getRenderer(), this->_texture, nullptr, &dstRect);
}

int Texture::getHeight() {
  return this->_height;
}

int Texture::getWidth() {
  return this->_width;
}
