#pragma once

#include "Constants.h"
#include "Math.h"
#include <vector>

class Game;

class Actor {
public:
  enum State {E_Active, E_Paused, E_Dead};
private:
  State m_State;

  // transform
  Vector2 m_Position; // center of actor
  float m_Scale; // 1.0f is 100%
  float m_Rotation; // rotation angle in RADIANS

  Vector2 m_Velocity;
  Vector2 m_ForwardVector;

  class Game* m_Game;

  std::vector<class Component*> m_Components; // sorted by update order
public:
  Actor(class Game* game);
  virtual ~Actor();
  void Update(float deltaTime);
  void UpdateComponents(float deltaTime); // updates all components
  virtual void UpdateActor(float deltaTime);

  void ProcessInput(const uint8_t* keyState); // called in Game not overridable
  void ProcessMouse(const uint32_t mouseState, const int x, const int y); // called in Game not overridable
  virtual void ActorInput(const uint8_t* keyState);

  Vector2 GetForward() const;

  State GetState() const;
  void SetState(State state);
  Vector2 GetPosition() const;
  void SetPosition(const Vector2& pos);
  float GetRotation() const;
  void SetRotation(float rotation);
  float GetScale() const;
  void SetScale(float scale);
  Vector2 GetVelocity() const;
  void SetVelocity(Vector2 velocity);
  Vector2 GetForwardVector() const;
  void SetForwardVector(Vector2 forwardVector);
  class Game* GetGame();

  void AddComponent(class Component* component);
  void RemoveComponent(class Component* component);

  void ClampToScreen(float& pos, int objHeight, int limit);
};
