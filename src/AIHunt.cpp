#include "AIHunt.h"
#include "AIComponent.h"
#include "Actor.h"
#include "Game.h"

#include <iostream> // todo remove

AIHunt::AIHunt(class AIComponent* owner)
  :AIState(owner)
{}

// override with behaviours for this state
void AIHunt::Update(float deltaTime)
{
  // if STAY IN HUNT:
  Actor* owner = m_Owner->GetOwner();
  Vector2 playerPos = owner->GetGame()->GetPlayerPosition();
  Vector2 toPlayer = playerPos - owner->GetPosition();

  owner->SetGoal(playerPos);
  // if player over x distance, switch to patrol
  if ( toPlayer.Length() > 300.0f )
  {
    m_Owner->ChangeState("Patrol");
  }

  //if ( distanceToPlayer.Length() < 100.0f)
  //{
  //  m_Owner->ChangeState("Attack");
  //}
}

void AIHunt::OnEnter()
{}

void AIHunt::OnExit()
{}

const char* AIHunt::GetName() const
{
  return "Hunt";
}
