#include "CollisionDetection.h"
#include "CircleComponent.h"

bool CollisionDetection::HasCollision(class CircleComponent* a, class CircleComponent* b)
{
  // TODO BREAKS because I'm attempting to change const returns ie GetCenter, GetRadius? 
  if (a == nullptr || b == nullptr) {return false;}
  // calc distance squared (no need to calc expensive square root)
  Vector2 diff = a->GetCenter() - b->GetCenter();
  float distSq = diff.LengthSq();

  // calc sum of radii squared
  float radiiSq = a->GetRadius() + b->GetRadius();

  return distSq <= (radiiSq * radiiSq);
}
