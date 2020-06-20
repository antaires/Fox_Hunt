#pragma once

#include "Constants.h"
#include <unordered_set>

class Entity {
private:
  std::unordered_set<Component*> m_Components;
public:
  Vector2 pos;
  Vector2 vel;

  void AddComponent(Component* comp);
  void RemoveComponent(Component* comp);
};
