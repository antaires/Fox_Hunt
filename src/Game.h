#pragma once

#include <SDL2/SDL.h>
#include "Actor.h"

class Game {
private:
  SDL_Window* m_Window;
  SDL_Renderer* m_Renderer;

  Uint32 m_TicksCount;
  bool m_IsRunning;

  Entity m_Fox;
  Entity m_Hunter;
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
