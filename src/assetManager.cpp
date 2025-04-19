#include "assetManager.h"
#include "texture.h"
#include "animation.h"
#include <filesystem>
#include <memory>
#include <spdlog/spdlog.h>

AssetManager::AssetManager(const std::string &path) {
  _basePath = std::filesystem::current_path();
  spdlog::debug("Initializing AssetManager");
  std::filesystem::path projectRoot = findProjectRoot(path);
  _basePath = projectRoot / path;
  spdlog::debug("AssetManager initialied with basePath: {}", _basePath.string());
}

std::filesystem::path AssetManager::findProjectRoot(const std::string &path) {
  std::filesystem::path currentPath = std::filesystem::current_path();

  while (!currentPath.empty()) {
    if (std::filesystem::exists(currentPath / "CMakeLists.txt") && std::filesystem::exists(currentPath / path)) {
      return currentPath;
    }
    std::filesystem::path parentPath = currentPath.parent_path();

    if (parentPath == currentPath) {
      // reached root filesystem
      break ;
    }

    currentPath = parentPath;
  }

  spdlog::warn("Failed to find project root, using current directory");
  return std::filesystem::current_path();
}

std::shared_ptr<Texture> AssetManager::loadTexture(const std::string &id, const std::string &filename, SDL_Renderer *renderer) {
  std::string fullPath = (_basePath / filename).string();

  std::shared_ptr<Texture> texture = createAsset<Texture>(id, fullPath, renderer);
  return texture;
}

bool AssetManager::removeTexture(const std::string &id) {
  return removeAsset<Texture>(id);
}

std::shared_ptr<Animation> AssetManager::loadAnimation(
  const std::string &id,
  const std::string &filename,
  SDL_Renderer *renderer,
  int frameWidth,
  int frameHeight,
  int frameCount,
  float frameTime
){
  std::string fullPath = (_basePath / filename).string();

  std::shared_ptr<Animation> animation = createAsset<Animation>(id, fullPath, renderer, frameWidth, frameHeight, frameCount, frameTime);
  return animation;
}

bool AssetManager::removeAnimation(const std::string &id) {
  return removeAsset<Animation>(id);
}
