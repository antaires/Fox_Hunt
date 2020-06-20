#include "Actor.h"

Actor::Actor(class Game* game): m_Game(game)
{}

Actor::~Actor()
{}

void Actor::Update(float deltaTime)
{
  // TODO : use fox update in game to update pos using velocity here
}

void Actor::UpdateComponents(float deltaTime)
{
    for (auto comp : m_Components)
    {
      comp->Update(deltaTime);
    }
}

void Actor::UpdateActor(float deltaTime) {}

// TODO: Getters / setters...
Actor::State Actor::GetState() const
{
  return m_State;
}

Vector2 Actor::GetPosition() const
{
  return m_Position;
}

float Actor::GetRotation() const
{
  return m_Rotation;
}

void Actor::AddComponent(class Component* component)
{
  m_Components.emplace_back(component);
}

void Actor::RemoveComponent(class Component* component)
{
  // TODO just set to nullptr and remove later?
  auto it = find(m_Components.begin(), m_Components.end(), component);
  if (it != m_Components.end())
  {
    m_Components.erase(it);
  }
}
