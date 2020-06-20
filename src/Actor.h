#pragma once

#include "Constants.h"
#include "Component.h"
#include <vector>

// TODO temp
class Game;

class Actor {
public:
  enum State {E_Active, E_Paused, E_Dead};
private:
  State m_State;
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

  // TODO: Getters / setters...
  State GetState() const;
  Vector2 GetPosition() const;
  float GetRotation() const;

  void AddComponent(class Component* component);
  void RemoveComponent(class Component* component);
};
