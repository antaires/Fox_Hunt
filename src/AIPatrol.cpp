#include "AIPatrol.h"
#include "AIComponent.h"
#include "Actor.h"

#include <iostream> // todo remove

AIPatrol::AIPatrol(class AIComponent* owner)
  :AIState(owner)
{
  // TODO set actor goal as current position
  m_Owner->GetOwner()->SetGoal(Vector2(30.0f, 30.0f));
}

// override with behaviours for this state
void AIPatrol::Update(float deltaTime)
{
  // TODO decide if i stay in patrol or change state

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
    }
    else
    {
      // if path empty then pick a new random target location (only in center of a cell grid)
      // and set goal using the m_Map
      m_Owner->GetMap()->GetPath(owner->GetPosition(), m_Owner->GetMap()->GetRandomOpenPosition(), m_Path);
      if (!m_Path.empty())
      {
        // path found, set next goal point as next node in path
        owner->SetGoal(m_Path.at(0));
        m_Path.erase(m_Path.begin() + 0);
      }
      else
      {
        // no path found, default to zero position & try again
        m_Path.push_back(Vector2(0.0f, 0.0f));
      }
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
