#include "Enemy.h"
#include "SDL2/SDL.h"
#include "AnimSpriteComponent.h"
#include "CircleComponent.h"
#include "Player.h"
#include "Game.h"

// TODO: add AI component for movement

Enemy::Enemy(class Game* game)
  : Actor(game)
  , m_Circle(nullptr)
{
  // TODO set up AI component (which extends Move Component)

  SetScale(0.25f);

  // set up animation component
  m_AnimSpriteComponent = new AnimSpriteComponent(this);
  std::vector<SDL_Texture*> anims = {
      game->GetTexture("assets/hunter01.png") // right fox : 0 - 3
  };
  m_AnimSpriteComponent->SetAnimTextures(anims);

  // set names and ranges of animations
  m_AnimSpriteComponent->SetAnimationClip("right", 0, 0, true);
  m_AnimSpriteComponent->SetAnimationClip("left", 0, 0, true);
  m_AnimSpriteComponent->SetAnimationClip("down", 0, 0, false);
  m_AnimSpriteComponent->SetAnimationClip("up", 0, 0, false);
  m_AnimSpriteComponent->SetAnimationClip("stillRight", 0, 0, false);
  m_AnimSpriteComponent->SetAnimationClip("stillLeft", 0, 0, false);
  m_AnimSpriteComponent->SetAnimationClip("stillUp", 0, 0, false);
  m_AnimSpriteComponent->SetAnimationClip("stillDown", 0, 0, false);

  // set actor heigth / width from texture and scale
  SetHeight(m_AnimSpriteComponent->GetTextureHeight() * GetScale());
  SetWidth(m_AnimSpriteComponent->GetTextureWidth() * GetScale());

  // set up circle component for collisions
  m_Circle = new CircleComponent(this);
  m_Circle->SetRadius( m_AnimSpriteComponent->GetTextureWidth() / 2 * GetScale());
}

void Enemy::UpdateActor(float deltaTime)
{
  Actor::UpdateActor(deltaTime);

  // TODO place all of this on AnimSpriteComponent, based on isMoving, isFiring, velocity, forwardVector
  // since NPCs will need this same logic
  bool isMoving = true;
  Vector2 velocity = GetVelocity();
  if (velocity.x == 0 && velocity.y == 0)
  {
    isMoving = false;
  }

  Vector2 forwardVector = Vector2::Normalize(GetForwardVector());
  Vector2 rightVector = Vector2(1, 0);
  float angle = Math::Acos(Vector2::Dot(rightVector, forwardVector)); //Math::Atan2(-forwardVector.y, forwardVector.x);
  bool isAbove = forwardVector.y < 0;

  // set animation based on movement and forward vector
  // TODO convert forwardVector to rotation angle, and add animations for vertical movement as well
  if (angle > 2)
  {
    if (isMoving)
    {
      m_AnimSpriteComponent->SetCurrentAnimationClip("left");
    } else {
      m_AnimSpriteComponent->SetCurrentAnimationClip("stillLeft");
    }
  }
  if (angle < 2)
  {
    if(isMoving)
    {
      m_AnimSpriteComponent->SetCurrentAnimationClip("right");
    } else {
      m_AnimSpriteComponent->SetCurrentAnimationClip("stillRight");
    }
  }
  if ( !isAbove && (angle < 2 && angle > 1) )
  {
    if(isMoving)
    {
      m_AnimSpriteComponent->SetCurrentAnimationClip("down");
    } else {
      m_AnimSpriteComponent->SetCurrentAnimationClip("stillDown");
    }
  }
  if ( isAbove && (angle < 2 && angle > 1) )
  {
    if(isMoving)
    {
      m_AnimSpriteComponent->SetCurrentAnimationClip("up");
    } else {
      m_AnimSpriteComponent->SetCurrentAnimationClip("stillUp");
    }
  }
}

class CircleComponent* Enemy::GetCircle() const
{
  return m_Circle;
}

// void Enemy::ProcessKeyboard(const uint8_t* state){}
