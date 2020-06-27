#include "Actor.h"
#include "Game.h"

Actor::Actor(class Game* game)
  : m_State(E_Active)
  , m_Position(Vector2(0.0f, 0.0f))
  , m_Scale(1.0f)
  , m_Rotation(0.0f)
  , m_Velocity(Vector2(0.0f, 0.0f))
  , m_ForwardVector(Vector2(0.0f, 0.0f))
  , m_Game(game)
{
  m_Game->AddActor(this);
}

Actor::~Actor()
{
  m_Game->RemoveActor(this);

  while(!m_Components.empty())
  {
    delete m_Components.back();
  }
}

void Actor::Update(float deltaTime)
{
  if(m_State == E_Active)
  {
    UpdateComponents(deltaTime);
    UpdateActor(deltaTime);
  }
}

void Actor::UpdateComponents(float deltaTime)
{
    for (auto comp : m_Components)
    {
      comp->Update(deltaTime);
    }
}

void Actor::UpdateActor(float deltaTime){}

void Actor::ProcessInput(const uint8_t* keyState)
{
  // called in Game not overridable
  if (m_State == E_Active)
  {
    for(auto comp: m_Components)
    {
      comp->ProcessInput(keyState);
    }
    ActorInput(keyState);
  }
}

void Actor::ProcessMouse(const uint32_t mouseState, const int x, const int y)
{
  // calle din game not overridable
  if (m_State == E_Active)
  {
    for(auto comp: m_Components)
    {
      comp->ProcessMouse(mouseState, x, y);
    }
  }
}

void Actor::ActorInput(const uint8_t* keyState)
{}

Vector2 Actor::GetForward() const
{
  // negate y axis for SDL (where +y is down)
  return Vector2(Math::Cos(m_Rotation), -Math::Sin(m_Rotation));
}


Actor::State Actor::GetState() const { return m_State;}

void Actor::SetState(State state) { m_State = state;}

Vector2 Actor::GetPosition() const { return m_Position;}

void Actor::SetPosition(const Vector2& pos) { m_Position = pos;}

float Actor::GetRotation() const { return m_Rotation;}

void Actor::SetRotation(float rotation){m_Rotation = rotation;}

float Actor::GetScale() const { return m_Scale; }

void Actor::SetScale(float scale) { m_Scale = scale; }

Vector2 Actor::GetVelocity() const { return m_Velocity; }

void Actor::SetVelocity(Vector2 velocity) { m_Velocity = velocity; }

Vector2 Actor::GetForwardVector() const { return m_ForwardVector; }

void Actor::SetForwardVector(Vector2 forwardVector) { m_ForwardVector = forwardVector; }

class Game* Actor::GetGame(){return m_Game;}

void Actor::AddComponent(class Component* component)
{
  // add components in sorted order
  int myOrder = component->GetUpdateOrder();
  auto it = m_Components.begin();
  for(; it != m_Components.end(); ++it)
  {
    if(myOrder < (*it)->GetUpdateOrder())
    {
      break;
    }
  }

  m_Components.insert(it, component);
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

void Actor::ClampToScreen(float& pos, int objHeight, int limit)
{
  if (pos < objHeight/2.0f){pos = objHeight/2.0f;}
  if (pos > limit - (objHeight / 2)){pos = limit - (objHeight/2);}
}
