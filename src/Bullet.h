#pragma once

#include "Actor.h"

class Bullet: public Actor
{
private:
  class SpriteComponent* m_SpriteComponent;
  class CircleComponent* m_Circle;
public:
  Bullet(class Game* game, Vector2 forwardVector);

  void UpdateActor(float deltaTime) override;
};
