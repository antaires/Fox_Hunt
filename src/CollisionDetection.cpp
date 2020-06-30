#include "CollisionDetection.h"
#include "CircleComponent.h"

bool CollisionDetection::HasCollision(class CircleComponent* a, class CircleComponent* b)
{
  if (a == nullptr || b == nullptr) {return false;}

  // calc distance squared (no need to calc expensive square root)
  Vector2 diff = Vector2(a->GetCenter().x, a->GetCenter().y) - Vector2(b->GetCenter().x, b->GetCenter().y);
  float distSq = diff.LengthSq();

  // calc sum of radii squared
  float radiiSq = a->GetRadius() + b->GetRadius();
  radiiSq *= radiiSq;

  return distSq <= radiiSq;
}
