#include "CollisionDetection.h"
#include "CircleComponent.h"

#include <iostream> // TODO remove

bool CollisionDetection::HasCollision(class CircleComponent* a, class CircleComponent* b)
{
  // TODO BREAKS because I'm attempting to change const returns ie GetCenter, GetRadius?
  if (a == nullptr || b == nullptr) {return false;}
  // calc distance squared (no need to calc expensive square root)

  /*
  Vector2 aCenter(a_->GetCenter().x, a_->GetCenter().y);
  Vector2 bCenter(b_->GetCenter().x, b_->GetCenter().y);

  Vector2 diff = aCenter - bCenter;
  */
  Vector2 diff = a->GetCenter() - b->GetCenter();
  float distSq = diff.LengthSq();

  // calc sum of radii squared
  float radiiSq = a->GetRadius() + b->GetRadius();

  return distSq <= (radiiSq * radiiSq);
}
