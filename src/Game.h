#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "Actor.h"
#include "SpriteComponent.h"

// TODO temporary
struct Entity {
  Vector2 pos;
  Vector2 vel;
};

class Game {
private:
  SDL_Window* m_Window;
  SDL_Renderer* m_Renderer;

  Uint32 m_TicksCount;
  bool m_IsRunning;
  bool m_UpdatingActors;

  std::vector<class Actor*> m_Actors;
  std::vector<class Actor*> m_PendingActors;
  std::vector<class SpriteComponent*> m_Sprites;
  std::unordered_map<std::string, SDL_Texture*> m_Textures;

  // Game specific
  Entity m_Fox;
  Entity m_Hunter;
  Vector2 m_WallPos;

  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();

  void DrawGameScene();
  void ClampToScreen(float& pos, int objHeight, int limit);

  void LoadData();
  void UnloadData();

public:
  Game();
  bool Initialize();
  void RunLoop();
  void ShutDown();

  void AddActor(class Actor* actor);
  void RemoveActor(class Actor* actor);
  void AddSprite(class SpriteComponent* sprite);
  void RemoveSprite(class SpriteComponent* sprite);

  SDL_Texture* GetTexture(const std::string& fileName);
};
