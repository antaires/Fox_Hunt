#pragma once

#include "Actor.h"

class Player: public Actor
{
private:
  float m_Speed;
  Vector2 m_Velocity;
public:
  Player(class Game* game);
  void UpdateActor(float deltaTime) override;
  void ProcessKeyboard(const uint8_t* state);
  float GetSpeed() const;
};
