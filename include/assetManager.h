#pragma once

#include <SDL3/SDL.h>
#include <filesystem>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>

class Texture;
class Animation;
struct SDLRendererCleaner;

class Asset {
public:
  virtual ~Asset() = default;
  virtual bool isLoaded() const = 0;
  virtual const std::string &getPath() const = 0;
};


class AssetManager {
public:
  using AssetPtr = std::shared_ptr<Asset>;

  explicit AssetManager(std::unique_ptr<SDL_Renderer> &renderer, const std::string &basePath = "assets/");
  explicit AssetManager(const std::string &basePath = "assets/");
  void setBasePath(const std::string &basePath);
  // no copy
  AssetManager(const AssetManager &src) = delete;
  AssetManager &operator=(const AssetManager &src) = delete;

  // can move, saw this online but honestly not sure what's the point
  AssetManager(AssetManager &&src) noexcept = default;
  AssetManager &operator=(AssetManager &&src) noexcept = default;

  void setRenderer(std::unique_ptr<SDL_Renderer, SDLRendererCleaner> &renderer);
  std::filesystem::path findProjectRoot(const std::string &path = "assets/");

  std::shared_ptr<Texture> loadTexture(const std::string &id, const std::string &filename);
  bool removeTexture(const std::string &id);

  std::shared_ptr<Animation> loadAnimation(const std::string &id,
                                           const std::string &filename,
                                           int frameWidth,
                                           int frameHeight,
                                           int frameCount,
                                           float frameTime);
  bool removeAnimation(const std::string &id);

private:
  std::filesystem::path _basePath;
  SDL_Renderer *_renderer;
  std::unordered_map<std::string, std::shared_ptr<Asset>> _assets;
};
