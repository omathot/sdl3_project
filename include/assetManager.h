#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <filesystem>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <type_traits>
#include <unordered_map>

class Texture;
class Animation;
// other assets forward declaration

class Asset {
public:
  virtual ~Asset() = default;
  virtual bool isLoaded() const = 0;
  virtual const std::string &getPath() const = 0;
};


class AssetManager {
public:
  using AssetPtr = std::shared_ptr<Asset>;

  explicit AssetManager(const std::string &basePath = "assets/");
  // no copy
  AssetManager(const AssetManager &src) = delete;
  AssetManager &operator=(const AssetManager &src) = delete;

  // can move, saw this online but honestly not sure what's the point
  AssetManager(AssetManager &&src) noexcept = default;
  AssetManager &operator=(AssetManager &&src) noexcept = default;

  std::filesystem::path findProjectRoot();

  std::shared_ptr<Texture> loadTexture(const std::string &id, const std::string &filename, SDL_Renderer *renderer);
  bool removeTexture(const std::string &id);

  std::shared_ptr<Animation> loadAnimation(const std::string &id,
                                           const std::string &filename,
                                           SDL_Renderer *renderer,
                                           int frameWidth,
                                           int frameHeight,
                                           int frameCount,
                                           float frameTime);
  bool removeAnimation(const std::string &id);

private:
  std::filesystem::path _basePath;
  std::unordered_map<std::string, std::shared_ptr<Asset>> _assets;




//--------------------------------------------------------------------------------------------------------------------------------------------
  // I HATE TEMPLATES MORE THAN YOU BUT THIS JUST MAKES SENSE HERE; AUDIOS, TEXTURES, ANIMATIONS, MESHES, FONTS, SHADERS ETC...
  template<typename T, typename... Args> std::shared_ptr<T> createAsset(const std::string &id,
                                                                        const std::string &path,
                                                                        SDL_Renderer *renderer,
                                                                        Args... args) {
    static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset");

    auto asset = std::make_shared<T>(path, args...);
    if constexpr (std::is_same_v<T, Texture>) {
      if (!asset->load(renderer)) {
        spdlog::error("Failed to load Texture: {}", path);
        return nullptr;
      }
    } else if constexpr(std::is_same_v<T, Animation>) { 
      if (!asset->load(renderer)) {
        spdlog::error("Failed to load Animation: {}", path);
        return nullptr;
      }
    } else { // other asset types
      
    }
    _assets[id] = asset;
    return asset;
  }

  template<typename T> bool removeAsset(const std::string &id) {
    static_assert(std::is_base_of<Asset, T>::value, "T must derive from Asset");

    auto it = _assets.find(id);
    if (it == _assets.end()) {
      spdlog::warn("Attempted to remove non-existent asset");
      return false;
    }

    // check T type matches what we're about to remove (extra but safer)
    auto asset_ptr = std::dynamic_pointer_cast<T>(it->second);
    if (!asset_ptr) {
      spdlog::error("Asset {} exists but is not of expected type", _assets[id]->getPath());
      return false;
    }

    std::string path = _assets[id]->getPath();
    _assets.erase(id);
    spdlog::debug("Asset {} removed", path);
    return true;
  }
};
