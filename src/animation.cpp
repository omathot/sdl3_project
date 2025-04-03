#include "animation.h"
#include <spdlog/spdlog.h>

Animation::Animation(const std::string &filePath, int frameWidth, int frameCount ,int framesCount, float frameTime) {
  _filePath = filePath;
  _count = framesCount;
  _frameTime = frameTime;
}

bool Animation::isLoaded() const {
  bool success = true;
  for (const auto &texture: _frames) {
    if (!texture->isLoaded()) {
      spdlog::error("Texture {} in animation not loaded", texture->getPath());
      success = false;
    }
  }
  return success;
}
