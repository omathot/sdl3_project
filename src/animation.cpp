#include "animation.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>
#include <cstdint>
#include <memory>
#include <spdlog/spdlog.h>

Animation::Animation(const std::string &filePath, uint16_t frameWidth,uint16_t frameHeight, int framesCount, float frameTime) {
  _filePath = filePath;
  _texture = nullptr;
  _framesCount = framesCount;
  _frameTime = frameTime;
  _frameWidth = frameWidth;
  _lastUpdateTime = SDL_GetTicks();
  _frameHeight = frameHeight;
  _isLoaded = false;

  _currentFrame = 0;
  _timeAccumulator = 0;
  _playBackState = PlayBackState::STOPPED;
  _isLooping = true;
  _startFrame = 0;
  _endFrame = 0;
  _playCount = 0;
  _targetPlayCount = 0;
}

bool Animation::load(SDL_Renderer *renderer) {
  _texture = std::make_shared<Texture>(_filePath);
  if (!_texture->load(renderer)) {
    spdlog::error("Failed to load animation {}", _texture->getPath());
    return false;
  }

  int textureWidth = _texture->getWidth();
  int textureHeight = _texture->getHeight();

  // if wasn't specific i calculate assuming horizontal layout
  if (_framesCount <= 0) {
    _framesCount = textureWidth / _frameWidth;
  }
  _endFrame = _framesCount - 1;
  _frameRects.clear(); // meh safety

  for (int i = 0; i < _framesCount; i++) {
    SDL_FRect frameRect = {
     float(i) * _frameWidth, // x position
     0,               // assuming single row
     float(_frameWidth),     // width of each frame
     float(textureHeight)     // height of each frame
    };
    _frameRects.push_back(frameRect);
  }

  _isLoaded = true;
  return _isLoaded;
}

bool Animation::isLoaded() const {
  return _isLoaded;
}

// void Animation::render(SDL_Renderer *renderer, float x, float y) {
//   if (!_isLoaded) {
//     spdlog::debug("returning early from animation render()");
//     return;
//   }
//   SDL_FRect srcRect = _frameRects[_currentFrame];
//   SDL_FRect dstRect = {x, y, float(_frameWidth), float(_frameHeight)};

//   spdlog::debug("rendering texture{} from rect x:{}, y:{}, w:{}, h:{}", _currentFrame, srcRect.x, srcRect.y, srcRect.w, srcRect.h);
//   SDL_RenderTexture(renderer, _texture->getTexture().get(), &_frameRects[_currentFrame], &dstRect);
// }

uint16_t Animation::getFrameHeight() const {
  return _frameHeight;
}

uint16_t Animation::getFrameWidth() const {
  return _frameWidth;
}

const std::string &Animation::getPath() const {
  return _filePath;
}

void Animation::play(bool loop, int playCount) {
    _isLooping = loop;
    _targetPlayCount = playCount;
    _playCount = 0;
    _playBackState = PlayBackState::PLAYING;
    resetTiming();
}

void Animation::playSingleShot() {
    play(false, 1);
}

void Animation::playMultiple(int times) {
  play(false, times);
}

void Animation::pause() {
  if (_playBackState == PlayBackState::PLAYING) {
    _playBackState = PlayBackState::PAUSED;
  }
}

void Animation::resume() {
  if (_playBackState == PlayBackState::PAUSED) {
    _playBackState = PlayBackState::PLAYING;
  }
}

void Animation::stop() {
  _playBackState = PlayBackState::STOPPED;
  reset();
}

void Animation::reset() {
  _currentFrame = _startFrame;
  _timeAccumulator = 0.0f;
  _playCount = 0;
}

void Animation::setFrame(int frameIndex) {
  if (frameIndex >= 0 && frameIndex <= _framesCount) {
    _currentFrame = frameIndex;
  }
}

void Animation::setFrameRange(int startFrame, int endFrame) {
  if (startFrame >= 0 && startFrame <= _framesCount && endFrame >= 0 && endFrame <= _framesCount && endFrame >= startFrame) {
    _startFrame = startFrame;
    _endFrame = endFrame;
    _currentFrame = startFrame;
  }
}

void Animation::setFrameTime(float frameTime) {
  if (frameTime > 0) {
    _frameTime = frameTime;
  }
}

void Animation::update() {
  if (!_isLoaded || _playBackState != PlayBackState::PLAYING) {
    return;
  }
  uint64_t currentTime = SDL_GetTicks();
  float deltaSeconds = (currentTime - _lastUpdateTime) / 1000.0f;
  _lastUpdateTime = currentTime;

  _timeAccumulator += deltaSeconds;
  while (_timeAccumulator >= _frameTime) {
    _timeAccumulator -= _frameTime;
    advanceFrame();
  }
}

void Animation::resetTiming() {
  _lastUpdateTime = SDL_GetTicks();
  _timeAccumulator = 0.0f;
}

void Animation::render(SDL_Renderer *renderer,
                         int x,
                         int y,
                         float rotation,
                         float scale,
                         bool flipX,
                         bool flipY
){
  if (!_isLoaded) {
    spdlog::debug("Returning early from animation render()");
    return;
  }

  float scaleWidth = static_cast<float>(scale * _frameWidth);
  float scaleHeight = static_cast<float>(scale * _frameHeight);

  SDL_FRect dstRect = {float(x), float(y), scaleWidth, scaleHeight};
  SDL_FRect srcRect = _frameRects[_currentFrame];
  spdlog::debug("rendering texture{} from rect x:{}, y:{}, w:{}, h:{}", _currentFrame, srcRect.x, srcRect.y, srcRect.w, srcRect.h);

  SDL_FlipMode flip = SDL_FLIP_NONE;
  if (flipX) {
    flip = static_cast<SDL_FlipMode>(flip | SDL_FLIP_HORIZONTAL);
  }
  if (flipY) {
    flip = static_cast<SDL_FlipMode>(flip | SDL_FLIP_VERTICAL);
  }

  SDL_RenderTextureRotated(renderer, _texture->getTexture().get(), &_frameRects[_currentFrame], &dstRect, rotation, NULL, flip);
}

void Animation::advanceFrame() {
  _currentFrame++;
  if (_currentFrame > _endFrame) {
    _playCount++;

    if (!_isLooping && _playCount >= _targetPlayCount && _targetPlayCount > 0) {
      _playBackState = PlayBackState::STOPPED;
      _currentFrame = _endFrame;
      return;
    }

    _currentFrame = _startFrame;
  }
}
