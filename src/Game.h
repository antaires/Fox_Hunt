#pragma once

#include <SDL2/SDL.h>

struct Vector2
{
  float x;
  float y;
};

class Game {
private:
  SDL_Window* m_Window;
  SDL_Renderer* m_Renderer;

  Uint32 m_TicksCount;
  bool m_IsRunning;

  Vector2 m_FoxPos;
  Vector2 m_FoxVel;
  Vector2 m_HunterPos;
  Vector2 m_HunterVel;
  Vector2 m_WallPos;

  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();

  void DrawGameScene();
  void ClampToScreen(float& pos, int objHeight, int limit);

public:
  Game();
  bool Initialize();
  void RunLoop();
  void ShutDown();
};
