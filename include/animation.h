#pragma once

#include <SDL3/SDL_render.h>
#include <cstdint>
#include <vector>
#include "texture.h"

class Animation : public Asset {
public:
  enum class PlayBackState {
    STOPPED,
    PLAYING,
    PAUSED,
  };

  Animation(const std::string &filePath, uint16_t frameWidth, uint16_t frameHeight, int framesCount, float frameTime = 0.1);

  bool load(SDL_Renderer *renderer);
  bool isLoaded() const override;
  const std::string &getPath() const override;
  // void render(SDL_Renderer *renderer, float x, float y);
  uint16_t getFrameWidth() const;
  uint16_t getFrameHeight() const;

  // play controls
  void play(bool loop = true, int playCount = 0);
  void playSingleShot();
  void playMultiple(int times);
  void pause();
  void resume();
  void stop();
  void reset();
  void setFrame(int frameIndex);
  void setFrameRange(int startFrame, int endFrame);
  void setFrameTime(float frameTime);
  void update();
  void resetTiming();
  void render(SDL_Renderer *renderer, int x, int y, float rotation = 0.0f, float scale = 1.0f, bool flipX = false, bool flipY= false);

private:
  std::string _filePath;
  std::shared_ptr<Texture> _texture;
  std::vector<SDL_FRect> _frameRects;

  uint8_t _currentFrame;
  uint8_t _framesCount;
  uint16_t _frameWidth;
  uint16_t _frameHeight;
  float _frameTime;
  float _timeAccumulator;
  uint64_t _lastUpdateTime;
  bool _isLoaded;

  PlayBackState _playBackState;
  bool _isLooping;
  uint8_t _startFrame;
  uint8_t _endFrame;
  int _playCount;
  int _targetPlayCount;

  // private helper
  void advanceFrame();
};

