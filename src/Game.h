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

  bool m_IsRunning;

  Vector2 m_FoxPos;
  Vector2 m_HunterPos;

  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();
  void DrawGameScene();
public:
  Game();
  bool Initialize();
  void RunLoop();
  void ShutDown();
};
