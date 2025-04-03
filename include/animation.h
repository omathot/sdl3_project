#pragma once

#include <cstdint>
#include <vector>
#include "texture.h"

class Animation : public Asset {
public:
  Animation(const std::string &filePath, int framesCount, uint16_t frameWidth, uint16_t frameHeight, float frameTime = 0.1);

  bool isLoaded() const override;
  const std::string &getPath() const override;

private:
  std::string _filePath;
  std::shared_ptr<Texture> _texture;
  std::vector<SDL_Rect> _frameRect;

  uint8_t _currentFrame;
  uint8_t _count;
  uint16_t _frameWidth;
  uint16_t _frameHeight;
  float _frameTime = 0.1;
};

