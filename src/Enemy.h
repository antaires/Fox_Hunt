#pragma once

#include "Actor.h"

class Enemy: public Actor
{
private:
  class AnimSpriteComponent* m_AnimSpriteComponent;
  class CircleComponent* m_Circle;
public:
  Enemy(class Game* game);
  void UpdateActor(float deltaTime) override;
  class CircleComponent* GetCircle() const;
  // void ProcessKeyboard(const uint8_t* state);
};
