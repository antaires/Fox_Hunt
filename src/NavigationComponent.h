#pragma once

#include "MoveComponent.h"
#include "Math.h"

class NavigationComponent: public MoveComponent
{
private:
public:
  NavigationComponent(class Actor* owner, int updateOrder = 10);
  void Update(float deltaTime) override;
};
