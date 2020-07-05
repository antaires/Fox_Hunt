#pragma once

#include "Actor.h"

class Enemy: public Actor
{
private:
  class AnimSpriteComponent* m_AnimSpriteComponent;
  class CircleComponent* m_Circle; // todo remove ?
  class RectangleComponent* m_Rectangle;
public:
  Enemy(class Game* game);
  void UpdateActor(float deltaTime) override;
  class CircleComponent* GetCircle() const;
  class RectangleComponent* GetRectangle() const;

  void HandleDeath() override;
  // void ProcessKeyboard(const uint8_t* state);
};
