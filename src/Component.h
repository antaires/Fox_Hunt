#pragma once

class Actor;

class Component {
private:
  Actor* mOwner;
  int m_UpdateOrder;
public:
  // lower update order -> update earlier
  Component(class Actor* owner, int updateOder = 100);
  virtual ~Component();
  virtual void Update(float deltaTime);
  int GetUpdateOrder() const {return m_UpdateOrder;}
};
