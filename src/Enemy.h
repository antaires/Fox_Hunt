#pragma once

#include "Actor.h"

class Enemy: public Actor
{
private:
  class AnimSpriteComponent* m_AnimSpriteComponent;

public:
  Enemy(class Game* game);
  void UpdateActor(float deltaTime) override;
  // void ProcessKeyboard(const uint8_t* state);
};
