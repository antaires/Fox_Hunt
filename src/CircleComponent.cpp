#include "CircleComponent.h"

CircleComponent::CircleComponent(class Actor* owner)
  :Component(owner)
  , m_Radius(0)
{}

void CircleComponent::SetRadius(float radius)
{
  m_Radius = radius;
}

float CircleComponent::GetRadius() const
{
  return m_Radius;
}

const Vector2& CircleComponent::GetCenter() const
{
  return m_Owner->GetPosition();
}

// global function
bool Intersect(const CircleComponent& a, const CircleComponent& b)
{
  // calc distance squared (no need to calc expensive square root)
  Vector2 diff = a.GetCenter() - b.GetCenter();
  float distSq = diff.LengthSq();

  // calc sum of radii squared
  float radiiSq = a.GetRadius() + b.GetRadius();
  radiiSq *= radiiSq;

  return distSq <= radiiSq;
}
