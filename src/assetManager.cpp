#include "assetManager.h"
#include "texture.h"
#include "animation.h"
#include "app.h"
#include <memory>


AssetManager::AssetManager(std::unique_ptr<SDL_Renderer> &renderer, const std::string &path) : _renderer(renderer.get()) {
  _basePath = std::filesystem::current_path();
  spdlog::debug("Initializing AssetManager");
  std::filesystem::path projectRoot = findProjectRoot(path);
  _basePath = projectRoot / path;
  spdlog::debug("AssetManager initialied with basePath: {}", _basePath.string());
}

AssetManager::AssetManager(const std::string &path) : _renderer(nullptr) {
  _basePath = std::filesystem::current_path();
  spdlog::debug("Initializing AssetManager");
  std::filesystem::path projectRoot = findProjectRoot(path);
  _basePath = projectRoot / path;
  spdlog::debug("AssetManager initialized with basePath: {}", _basePath.string());
}

void AssetManager::setRenderer(std::unique_ptr<SDL_Renderer, SDLRendererCleaner> &renderer) {
  _renderer = renderer.get();
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

std::shared_ptr<Texture> AssetManager::loadTexture(const std::string &id, const std::string &filename) {
  std::string fullPath = (_basePath / filename).string();

  std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture(fullPath));
  if (!texture->load(_renderer)) {
    spdlog::error("Failed to load Texture {}: ", fullPath);
    return nullptr;
  }
  _assets[id] = texture;
  return texture;
}

bool AssetManager::removeTexture(const std::string &id) {
  auto it = _assets.find(id);
  if (it == _assets.end()) {
    spdlog::warn("Attempted to remove non-existent asset");
    return false;
  }
  std::string path = _assets[id]->getPath();
  _assets.erase(id);
  spdlog::debug("Asset {} removed", path);
  return true;
}

std::shared_ptr<Animation> AssetManager::loadAnimation(
  const std::string &id,
  const std::string &filename,
  int frameWidth,
  int frameHeight,
  int frameCount,
  float frameTime
){
  std::string fullPath = (_basePath / filename).string();

  std::shared_ptr<Animation> animation = std::make_shared<Animation>(Animation(fullPath, frameWidth, frameHeight, frameCount, frameTime));
  if (!animation->load(_renderer)) {
    spdlog::error("Failed to load Texture: {}", fullPath);
    return nullptr;
  }
  _assets[id] = animation;
  return animation;
}

bool AssetManager::removeAnimation(const std::string &id) {
  auto it = _assets.find(id);
  if (it == _assets.end()) {
    spdlog::warn("Attempteed to remove non-existent asset");
    return false;
  }
  std::string path = _assets[id]->getPath();
  _assets.erase(id);
  spdlog::debug("Asset {} removed", path);
  return true;
}
