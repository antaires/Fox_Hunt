#include "Component.h"

Component::Component(class Actor* owner, int updateOrder)
  : m_Owner(owner)
  , m_UpdateOrder(updateOrder)
{
  // add to actor's vector of components
  m_Owner->AddComponent(this);
}

Component::~Component()
{
  m_Owner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{}

int Component::GetUpdateOrder() const {return m_UpdateOrder;}
