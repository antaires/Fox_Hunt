#pragma once

#include "Actor.h"

class Component {
protected:
  class Actor* m_Owner;
  int m_UpdateOrder;
public:
  // lower update order -> update earlier
  Component(class Actor* owner, int updateOrder = 100);
  virtual ~Component();
  virtual void Update(float deltaTime);
  int GetUpdateOrder() const;
};
