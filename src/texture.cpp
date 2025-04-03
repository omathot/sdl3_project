#include "texture.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <spdlog/spdlog.h>

Texture::Texture(const std::string &path) : _path(path) {
  this->_texture = nullptr;
  this->_width = 0;
  this->_height = 0;
}

bool Texture::load(SDL_Renderer *renderer) {
  _texture.reset();

  SDL_Surface *loadedSurface = IMG_Load(_path.c_str());
  if (!loadedSurface) {
    spdlog::error("Failed to IMG_Load texture");
    return false;
  }

  _texture.reset(SDL_CreateTextureFromSurface(renderer, loadedSurface), SDLTextureCleaner());
  _width = loadedSurface->w;
  _height = loadedSurface->h;

  SDL_DestroySurface(loadedSurface);

  return _texture != nullptr;
}

bool Texture::isLoaded() const {
  return _texture != nullptr;
}

const std::string &Texture::getPath() const {
  return _path;
}

std::shared_ptr<SDL_Texture> Texture::getTexture() const {
  return _texture;
}

int Texture::getHeight() const {
  return this->_height;
}

int Texture::getWidth() const {
  return this->_width;
}
