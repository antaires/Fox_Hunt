#pragma once

#include "Constants.h"
#include "Math.h"
#include "Game.h"
#include <vector>

class Actor {
public:
  enum State {E_Active, E_Paused, E_Dead};
private:
  State m_State;

  // transform
  Vector2 m_Position; // center of actor
  float m_Scale; // 1.0f is 100%
  float m_Rotation; // rotation angle in RADIANS

  std::vector<class Component*> m_Components; // sorted by update order
  class Game* m_Game;
public:
  Actor(class Game* game);
  virtual ~Actor();
  void Update(float deltaTime);
  void UpdateComponents(float deltaTime); // updates all components
  virtual void UpdateActor(float deltaTime);

  State GetState() const;
  void SetState(State state);
  Vector2 GetPosition() const;
  void SetPosition(const Vector2& pos);
  float GetRotation() const;
  void SetRotation(float rotation);
  float GetScale() const;
  void SetScale(float scale);
  class Game* GetGame();

  void AddComponent(class Component* component);
  void RemoveComponent(class Component* component);
};
