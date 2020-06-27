#include "MoveComponent.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
  :Component(owner, updateOrder)
  , m_AngularSpeed(0)
  , m_ForwardSpeed(FOX_SPEED)
  , m_Velocity(Vector2(0.0f, 0.0f))
  , m_ForwardVector(Vector2(0.0f, 0.0f))
  , m_ClampToScreen(false)
{}

void MoveComponent::Update(float deltaTime)
{
  // set actor postion using velocity
  Vector2 pos = m_Owner->GetPosition();
  if (m_Velocity.y != 0)
  {
    pos.y += m_Velocity.y * m_ForwardSpeed * deltaTime;
    if (m_ClampToScreen)
    {
      m_Owner->ClampToScreen(pos.y, FOX_HEIGHT, SCREEN_HEIGHT);
    }
  }
  if (m_Velocity.x != 0)
  {
    pos.x += m_Velocity.x * m_ForwardSpeed * deltaTime;
    if (m_ClampToScreen)
    {
      m_Owner->ClampToScreen(pos.x, FOX_WIDTH, SCREEN_WIDTH);
    }
  }
  m_Owner->SetVelocity(m_Velocity); // animation comp uses this variable
  m_Owner->SetPosition(pos);

  // set actor forwardVector
  m_Owner->SetForwardVector(m_ForwardVector);

  /* not used currently - maybe use this for enemy
  if (!Math::NearZero(m_AngularSpeed))
  {
    float rot = m_Owner->GetRotation();
    rot += m_AngularSpeed * deltaTime;
    m_Owner->SetRotation(rot);
  }

  if (!Math::NearZero(m_ForwardSpeed))
  {
    Vector2 pos = m_Owner->GetPosition();
    pos += m_Owner->GetForward() * m_ForwardSpeed * deltaTime;
    m_Owner->SetPosition(pos);
  }*/
}

void MoveComponent::SetVelocity(Vector2 velocity) { m_Velocity = velocity;}

Vector2 MoveComponent::GetVelocity() const {return m_Velocity;}

void MoveComponent::SetForwardVector(Vector2 forwardVector) { m_ForwardVector = forwardVector;}

Vector2 MoveComponent::GetForwardVector() const { return m_ForwardVector; }

float MoveComponent::GetAngularSpeed() const {return m_AngularSpeed;}

void MoveComponent::SetAngularSpeed(float angularSpeed) { m_AngularSpeed = angularSpeed;}

void MoveComponent::SetClampToScreen(bool clampToScreen) { m_ClampToScreen = clampToScreen;}
