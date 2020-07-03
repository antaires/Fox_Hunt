#include "CollisionDetection.h"
#include "CircleComponent.h"
#include "RectangleComponent.h"
#include "Math.h"

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

bool CollisionDetection::HasCollision(class RectangleComponent* a, class RectangleComponent* b)
{
  // TODO : test
  if (a == nullptr || b == nullptr) {return false;}

  Vector2 aPos = a->GetTopCornerPosition();
  Vector2 bPos = b->GetTopCornerPosition();

  if (aPos.x < bPos.x + b->GetWidth() &&
      aPos.x + a->GetWidth() > bPos.x &&
      aPos.y < bPos.y + b->GetHeight() &&
      aPos.y + a->GetHeight() > bPos.y)
    {
      return true;
    }

  return false;
}
