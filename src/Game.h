#pragma once

#include <SDL2/SDL.h>
#include <vector>
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
  std::vector<Actor*> m_Actors;
  std::vector<Actor*> m_PendingActors;

  std::vector<SpriteComponent*> m_Sprites;

  // TODO temporary
  Entity m_Fox;
  Entity m_Hunter;
  Vector2 m_WallPos;

  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();

  void DrawGameScene();
  void ClampToScreen(float& pos, int objHeight, int limit);
  void AddActor(Actor* actor);
  void RemoveActor(Actor* actor);

  SDL_Texture* LoadTexture(const char* fileName);
  void LoadData();
  void AddSprite(SpriteComponent* sprite);

public:
  Game();
  bool Initialize();
  void RunLoop();
  void ShutDown();
};
