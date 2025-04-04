#include "player.h"
#include "animation.h"
#include "assetManager.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <cstdlib>

Player::Player(AssetManager &mgr, SDL_Renderer *renderer) {
  _username = "JohnnyBoi";
  _pos = {300, 400, 0};
  _velocity = {0, 0};
  _facingRight = true;
  _grounded = true;
  _touchingWall = false;
  _currentState = "idle";

  _animations["idle"] = mgr.loadAnimation("player_idle", "V1/Player Idle/Player Idle 48x48.png", renderer, 48, 48, 10, 0.1);
  _animations["run"] = mgr.loadAnimation("player_run", "V1/Player Run/Player Run 48x48.png", renderer, 48, 48, 8, 0.1);
  _animations["jump"] = mgr.loadAnimation("player_jump", "V1/Player Jump/Player Jump 48x48.png", renderer, 48, 48, 3, 0.1);
  _animations["land"] = mgr.loadAnimation("player_land", "V1/Player Land/Player Land 48x48.png", renderer, 48, 48, 9, 0.1);
  _animations["wall_land"] = mgr.loadAnimation("player_wall_land", "V1/Player Wall Land/Player Wall Land 48x48.png", renderer, 48, 48, 6, 0.1);
  _animations["wall_slide"] = mgr.loadAnimation("player_wall_slide", "V1/Player Wall Slide/Player Wall Slide 48x48.png", renderer, 48, 48, 3, 0.1);
  _animations["dash"] = mgr.loadAnimation("player_dash", "V1/Player Dash/Player Dash 48x48.png", renderer, 48, 48, 9, 0.1);
  registerStates();
  changeState("idle");
}

void Player::registerStates() {
  registerIdleState();
  registerRunState();
}

void Player::changeState(const std::string &stateName) {
  if (_currentState == stateName) {
    return;
  }

  
  _currentState = stateName;
  auto enterIt = _enterFunctions.find(stateName);
  if (enterIt != _enterFunctions.end()) {
    enterIt->second(this);
  }
}

void Player::update(float deltaTime) {
  _pos.x *= _velocity.x * deltaTime;
  _pos.y *= _velocity.y * deltaTime;

  auto updateIt = _updateFunctions.find(_currentState);
  if (updateIt != _updateFunctions.end()) {
    updateIt->second(this, deltaTime);
  }
}

void Player::handleInput(const SDL_Event &event) {
  auto inputIt = _inputFunctions.find(_currentState);
  if (inputIt != _inputFunctions.end()) {
    inputIt->second(this, event);
  }
}

void Player::render(SDL_Renderer *renderer) {
  auto renderIt = _renderFunctions.find(_currentState);
  if (renderIt != _renderFunctions.end()) {
    renderIt->second(this, renderer);
  }
}



// state registrations
/*
  could reduce input handling code duplication by taking the most common inputs and put them in helper fn
  or make a base ground state that handles common input
*/
void Player::registerIdleState() {
  _enterFunctions["idle"] = [this](Player *player) {
    _animations["idle"]->play();
  };

  _updateFunctions["idle"] = [this](Player *player, float deltaTime) {
    _animations["idle"]->update();
    if (!_grounded) {
      changeState("jump"); // gameplay consequence (player doesn't need to press jump to fall off a ledge and be in jump state)
    } else if (std::abs(_velocity.x) > 0.1) {
      changeState("run"); // if player could be pushed, wouldn't need to input movement to get to "run"
    }
  };

  _renderFunctions["idle"] = [this](Player *player, SDL_Renderer *renderer) {
    _animations["idle"]->render(renderer, _pos.x, _pos.y, 0, 1.0f, !_facingRight, false);
  };

  _inputFunctions["idle"] = [this](Player *player, const SDL_Event &event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
      if (event.key.key == SDLK_LEFT || event.key.key == SDLK_A) {
        _velocity.x = -100;
        _facingRight = false;
        changeState("run");
      } else if (event.key.key == SDLK_RIGHT || event.key.key == SDLK_D) {
        _velocity.x = 100;
        _facingRight = true;
        changeState("run");
      } else if (event.key.key == SDLK_SPACE && _grounded) {
        _velocity.y = -300;
        _grounded = false;
        changeState("jump");
      }
    }
  };
}

void Player::registerRunState() {
  _enterFunctions["run"] = [this](Player *player) {
    _animations["run"]->play();
  };

  _updateFunctions["run"]= [this](Player *player, float deltaTime) {
    _animations["run"]->update();
    if (!_grounded) {
      changeState("jump");
    } else if (std::abs(_velocity.x) < 0.1f) {
      changeState("idle");
    }
  };

  _renderFunctions["run"] = [this](Player *player, SDL_Renderer *renderer) {
    _animations["run"]->render(renderer, _pos.x, _pos.y, 0, 1.0f, !_facingRight, false);
  };

  _inputFunctions["run"] = [this](Player *player, const SDL_Event &event) {
    if (event.type == SDL_EVENT_KEY_DOWN) {
      if (event.key.key == SDLK_A) {
        _velocity.x = -100;
        _facingRight = false;
      } else if (event.key.key == SDLK_D) {
        _velocity.x = 100;
        _facingRight = true;
      } else if (event.key.key == SDLK_SPACE && _grounded) {
        _velocity.y = -300;
        _grounded = false;
        changeState("jump");
      }
    }
  };
}


