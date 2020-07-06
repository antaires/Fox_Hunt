#include "AIPatrol.h"

AIPatrol::AIPatrol(class AIComponent* owner)
  :AIState(owner)
  , m_Target(Vector2(0.0f, 0.0f))
{}

// override with behaviours for this state
void AIPatrol::Update(float deltaTime)
{
  // TODO pick a random target location and walk there
  // TODO need to use path finding (as does hunt / attack )

  // if arrived within x distance of target location,
  // (or haven't moved in x frames) then pick new target

  // TODO if 'see' player using line-of-sight
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
