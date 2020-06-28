#pragma once

#include "Actor.h"

class Player: public Actor
{
private:
  class AnimSpriteComponent* m_AnimSpriteComponent;

public:
  Player(class Game* game);
  void UpdateActor(float deltaTime) override;
  void ProcessKeyboard(const uint8_t* state);
};

// TODO add global func to access player actor pointer
// for enemy AI etc  
