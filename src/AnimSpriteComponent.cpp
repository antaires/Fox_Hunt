#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(class Actor* owner, int drawOrder)
  :SpriteComponent(owner, drawOrder)
  , m_CurrentFrame(0.0f)
  , m_AnimFPS(24.0f)
{}

void AnimSpriteComponent::Update(float deltaTime)
{
  SpriteComponent::Update(deltaTime);

  if(m_AnimTextures.size() > 0)
  {
    // update current frame based on frame rate and delta time
    m_CurrentFrame += m_AnimFPS * deltaTime;

    // wrap current frame if needed
    while(m_CurrentFrame >= m_AnimTextures.size())
    {
      m_CurrentFrame -= m_AnimTextures.size();
    }

    // set the current texture
    SetTexture(m_AnimTextures[static_cast<int>(m_CurrentFrame)]);
  }
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
  m_AnimTextures = textures;
  if(m_AnimTextures.size() > 0)
  {
    // set active texture to first frame
    m_CurrentFrame = 0.0f;
    SetTexture(m_AnimTextures[0]);
  }
}

float AnimSpriteComponent::GetAnimFPS() const { return m_AnimFPS; }

void AnimSpriteComponent::SetAnimFPS(float fps) { m_AnimFPS = fps; }
