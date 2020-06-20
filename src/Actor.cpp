#include "Actor.h"

Actor::Actor(class Game* game)
{}

Actor::~Actor() {}

void Actor::Update(float deltaTime){}
void Actor::UpdateComponents(float deltaTime){} // updates all components
void Actor::UpdateActor(float deltaTime) {}

// TODO: Getters / setters...
Actor::State Actor::GetState() const
{
  return m_State;
}

void Actor::AddComponent(class Component* component)
{}

void Actor::RemoveComponent(class Component* component)
{}
