#include "AIPatrol.h"
#include "AIComponent.h"
#include "Actor.h"

#include <iostream> // todo remove

AIPatrol::AIPatrol(class AIComponent* owner)
  :AIState(owner)
{
  // TODO set actor goal as current position
  // TESTING
  Vector2 a(0.0f, 600.0f);
  Vector2 b(600.f, 0.0f);

  m_Path.push_back(a);
  m_Path.push_back(b);

  m_Owner->GetOwner()->SetGoal(Vector2(30.0f, 30.0f));
}

// override with behaviours for this state
void AIPatrol::Update(float deltaTime)
{
  // decide if i stay in patrol or change state

  // if STAY IN PATROL:
  Actor* owner = m_Owner->GetOwner();
  Vector2 diff = owner->GetPosition() - owner->GetGoal();
  Vector2 prevPos = owner->GetPrevPosition();
  Vector2 pos = owner->GetPosition();
  if ( diff.Length() < 50.0f || (prevPos.x == pos.x && prevPos.y == pos.y) )
  {
    // set as next in vector
    if (!m_Path.empty())
    {
      owner->SetGoal(m_Path.at(0));
      m_Path.erase(m_Path.begin() + 0); // erase first element
    } else {
      // if path empty then pick a new random target location (only in center of a cell grid)
      // and set goal using the m_Map
      // TODO
    }
  }

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
