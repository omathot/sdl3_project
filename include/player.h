#pragma once

#include <SDL3/SDL_render.h>
#include <functional>
#include <string>
#include <memory>
#include <unordered_map>
#include "common.h"

class Animation;
class AssetManager;

class Player {
public:
  Player(AssetManager &mgr);

  void registerStates();
  void changeState(const std::string &stateName);
  void update(float deltaTime);
  void handleInput(const SDL_Event &event);
  void render(SDL_Renderer *renderer);

private:
  std::string _username;
  Vec3 _pos;
  Vec2 _velocity;
  bool _facingRight;
  bool _grounded;
  bool _touchingWall;

  std::string _currentState;
  std::unordered_map<std::string, std::shared_ptr<Animation>> _animations;

  // State handlers (trying this structure)
  using StateUpdateFn = std::function<void(Player *, float)>;
  using StateRenderFn = std::function<void(Player *, SDL_Renderer *)>;
  using StateEnterFn = std::function<void(Player*)>;
  using StateInputFn = std::function<void(Player *, const SDL_Event &)>;

  std::unordered_map<std::string, StateUpdateFn> _updateFunctions;
  std::unordered_map<std::string, StateRenderFn> _renderFunctions;
  std::unordered_map<std::string, StateEnterFn> _enterFunctions;
  std::unordered_map<std::string, StateInputFn> _inputFunctions;

  // state registrations
  void registerIdleState();
  void registerRunState();
  void registerJumpState();
  void registerLandState();
  void registerWallLandState();
  void registerWallSlideState();
  void registerDashState();
};
