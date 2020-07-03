#pragma once

class CollisionDetection {
public:
  static bool HasCollision(class CircleComponent* a, class CircleComponent* b);
  static bool HasCollision(class RectangleComponent* a, class RectangleComponent* b);
  // TODO make HasCollision(a, b) for every combination of colliders
};
