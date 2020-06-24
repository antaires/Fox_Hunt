#include "Player.h"
#include "SDL2/SDL.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

Player::Player(class Game* game)
  : Actor(game)
  , m_Speed(FOX_SPEED)
  , m_Velocity(Vector2(0.0f, 0.0f))
{
  m_AnimSpriteComponent = new AnimSpriteComponent(this);
  std::vector<SDL_Texture*> anims = {
      game->GetTexture("assets/fox01.png") // right fox : 0 - 3
    , game->GetTexture("assets/fox02.png")
    , game->GetTexture("assets/fox03.png")
    , game->GetTexture("assets/fox04.png")
    , game->GetTexture("assets/leftFox01.png") // left fox: 4 - 7
    , game->GetTexture("assets/leftFox02.png")
    , game->GetTexture("assets/leftFox01.png")
    , game->GetTexture("assets/leftFox03.png")
    , game->GetTexture("assets/downFox01.png")  // down fox: 8
    , game->GetTexture("assets/upFox01.png")  // up fox : 9
  };
  m_AnimSpriteComponent->SetAnimTextures(anims);

  // set names and ranges of animations
  m_AnimSpriteComponent->SetAnimationClip("right", 0, 3, true);
  m_AnimSpriteComponent->SetAnimationClip("left", 4, 7, true);
  m_AnimSpriteComponent->SetAnimationClip("down", 8, 8, false);
  m_AnimSpriteComponent->SetAnimationClip("up", 9, 9, false);
}

void Player::UpdateActor(float deltaTime)
{
  Actor::UpdateActor(deltaTime);
  Vector2 pos = Actor::GetPosition();
  if (m_Velocity.y != 0)
  {
    pos.y += m_Velocity.y * m_Speed * deltaTime;
    Actor::ClampToScreen(pos.y, FOX_HEIGHT, SCREEN_HEIGHT);
  }
  if (m_Velocity.x != 0)
  {
    pos.x += m_Velocity.x * m_Speed * deltaTime;
    Actor::ClampToScreen(pos.x, FOX_WIDTH, SCREEN_WIDTH);
  }
  Actor::SetPosition(pos);

  // TODO : play animation based on character direction
  if (m_Velocity.x < 0)
  {
    m_AnimSpriteComponent->SetCurrentAnimation("left");
  }
  if (m_Velocity.x > 0)
  {
    m_AnimSpriteComponent->SetCurrentAnimation("right");
  }
  if (m_Velocity.y > 0 )
  {
    m_AnimSpriteComponent->SetCurrentAnimation("down");
  }
  if (m_Velocity.y < 0 )
  {
    m_AnimSpriteComponent->SetCurrentAnimation("up");
  }
}

void Player::ProcessKeyboard(const uint8_t* state)
{
  m_Velocity.x = 0.0f; m_Velocity.y = 0.0f;

  if (state[SDL_SCANCODE_W])
  {
    m_Velocity.y -= 1;
  }
  if (state[SDL_SCANCODE_S])
  {
    m_Velocity.y += 1;
  }
  if (state[SDL_SCANCODE_A])
  {
    m_Velocity.x -= 1;
  }
  if (state[SDL_SCANCODE_D])
  {
    m_Velocity.x += 1;
  }
}

float Player::GetSpeed() const { return m_Speed; }
