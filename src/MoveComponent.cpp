#include "MoveComponent.h"

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
  :Component(owner, updateOrder)
  , m_AngularSpeed(0)
  , m_ForwardSpeed(0)
{}

void MoveComponent::Update(float deltaTime)
{
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
  }
}

float MoveComponent::GetAngularSpeed() const {return m_AngularSpeed;}
float MoveComponent::GetForwardSpeed() const {return m_ForwardSpeed;}
void MoveComponent::SetAngularSpeed(float angularSpeed) { m_AngularSpeed = angularSpeed;}
void MoveComponent::SetForwardSpeed(float forwardSpeed) { m_ForwardSpeed = forwardSpeed;}
