#pragma once

#include <SDL3/SDL_render.h>
#include <assetManager.h>
#include <memory>
#include <string>
#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>

class App;

struct SDLTextureCleaner {
  void operator()(SDL_Texture *texture) {
    if (texture) {
      spdlog::debug("Cleaning texture");
      SDL_DestroyTexture(texture);
    }
  }
};

class Texture : public Asset {
public:
  Texture() = default;
  explicit Texture(const std::string &path);

  bool load(SDL_Renderer *renderer);
  bool isLoaded() const override;
  const std::string &getPath() const override;
  std::shared_ptr<SDL_Texture> getTexture() const;

  int getWidth() const;
  int getHeight() const;

private:
  std::shared_ptr<SDL_Texture> _texture;
  std::string _path;
  int _width;
  int _height;
};
