#include "SpriteComponent.h"
#include <cmath>

SpriteComponent::SpriteComponent(class Actor* owner, int drawOrder = 100)
  : m_DrawOrder(drawOrder)
{
  m_Owner = owner;
}

SpriteComponent::~SpriteComponent(){}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
  if (m_Texture)
  {
    SDL_Rect r;
    r.w = static_cast<int>(m_TextureWidth * m_Owner->GetScale());
    r.h = static_cast<int>(m_TextureHeight * m_Owner->GetScale());
    r.x = static_cast<int>(m_Owner->GetPosition().x - r.w / 2);
    r.y = static_cast<int>(m_Owner->GetPosition().y - r.h / 2);

    SDL_RenderCopyEx(
       m_Renderer
      , m_Texture
      , nullptr
      , &r
      , -Math::ToDegrees(m_Owner->GetRotation())
      , nullptr
      , SDL_FLIP_NONE
    );
  }
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
  m_Texture = texture;
  SDL_QueryTexture(texture, nullptr, nullptr, &m_TextureWidth, &m_TextureHeight);
}

int SpriteComponent::GetDrawOrder() const {return m_DrawOrder;}
int SpriteComponent::GetTextureHeight() const {return m_TextureHeight;}
int SpriteComponent::GetTextureWidth() const {return m_TextureWidth;}
