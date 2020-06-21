#pragma once

#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent
{
private:
  std::vector<SDL_Texture*> m_AnimTextures;
  float m_CurrentFrame;
  float m_AnimFPS;
public:
  AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
  void Update(float deltaTime) override;
  void SetAnimTextures(const std::vector<SDL_Texture*>& textures);

  float GetAnimFPS() const;
  void SetAnimFPS(float fps);
};
