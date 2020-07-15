#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "Actor.h"
#include "Player.h"
#include "Enemy.h"
#include "SpriteComponent.h"


class Game {
private:
  SDL_Window* m_Window;
  SDL_Renderer* m_Renderer;

  bool m_IsRunning;
  bool m_UpdatingActors;
  Uint32 m_TicksCount;
  class Map* m_Map;

  // Game specific
  Player* m_Player;
  std::vector<Enemy*> m_Enemies;

  std::vector<class Actor*> m_Actors;
  std::vector<class Actor*> m_PendingActors;
  std::vector<class SpriteComponent*> m_Sprites;
  std::unordered_map<std::string, SDL_Texture*> m_Textures;

  void ProcessInput();
  void UpdateGame();
  void HandleCollisions();
  void GenerateOutput();

  void LoadData();
  void UnloadData();

public:
  Game();
  bool Initialize();
  void RunLoop();
  void ShutDown();

  std::vector<Enemy*> GetEnemies(); // TODO is this const?

  void AddActor(class Actor* actor);
  void RemoveActor(class Actor* actor);
  void AddSprite(class SpriteComponent* sprite);
  void RemoveSprite(class SpriteComponent* sprite);
  void SetMap(class Map* map);
  Vector2 GetPlayerPosition() const;
  class Player* GetPlayer();

  SDL_Texture* GetTexture(const std::string& fileName);

  bool CollidesWithBarrier(Vector2 pos, float width, float height);
};
