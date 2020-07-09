#include "AIPatrol.h"

AIPatrol::AIPatrol(class AIComponent* owner)
  :AIState(owner)
  , m_Target(Vector2(0.0f, 0.0f))
{}

// override with behaviours for this state
void AIPatrol::Update(float deltaTime)
{
  // decide if i stay in patrol or change state

  // if position == goal, pick a new random target location and set goal
  // using the m_Map

  // TODO if 'see' player using line-of-sight (collision ray, player)
  // if player within x distance, switch to hunt / attack
  //m_Owner->ChangeState("Hunt");
}

void AIPatrol::OnEnter()
{
  // pick random target location
}

void AIPatrol::OnExit()
{}

const char* AIPatrol::GetName() const
{
  return "Patrol";
}
