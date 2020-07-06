#pragma once

#include "Component.h"
#include <unordered_map>
#include <string>

class AIComponent: public Component
{
private:
  // maps name of state to AIState instance
  std::unordered_map<std::string, class AIState*> m_StateMap;
  class AIState* m_CurrentState;
public:
  AIComponent(class Actor* owner);

  void Update(float deltaTime) override;
  void ChangeState(const std::string& name);

  // add new state to the map
  void RegisterState(class AIState* state);
};
