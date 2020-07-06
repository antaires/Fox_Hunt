#pragma once

#include "AIState.h"
#include "Math.h"

class AIPatrol: public AIState
{
private:
  Vector2 m_Target;
public:
  AIPatrol(class AIComponent* owner);

  // override with behaviours for this state
  void Update(float deltaTime) override;
  void OnEnter() override;
  void OnExit() override;

  const char* GetName() const override;
};
