#include "Bullet.h"
#include "CollisionDetection.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CircleComponent.h"
#include "Game.h"

#include <iostream> // todo remove

// bullet class for player to hit and kill enemies
Bullet::Bullet(class Game* game, Vector2 forwardVector)
  :Actor(game)
  {
    SetScale(0.05);

    // set up sprite component
    m_SpriteComponent = new SpriteComponent(this);
    SDL_Texture* sprite = GetGame()->GetTexture("assets/playerBullet01.png");
    m_SpriteComponent->SetTexture(sprite);

    // move component
    MoveComponent* mc = new MoveComponent(this);
    mc->SetVelocity(forwardVector);

    // set up circle component for collisions
    m_Circle = new CircleComponent(this);
    m_Circle->SetRadius( m_SpriteComponent->GetTextureWidth() / 2 * GetScale());
  }

void Bullet::UpdateActor(float deltaTime)
{
  std::cout<<"\nbullet velocity: "<< GetVelocity().x <<" "<< GetVelocity().y;

  // do you hit an enemy?
  for(auto enemy : GetGame()->GetEnemies())
  {
    if( CollisionDetection::HasCollision(enemy->GetCircle(), m_Circle) )
    {
      enemy->SetState(Actor::E_Dead);
      SetState(Actor::E_Dead);
      break;
    }
  }
}
