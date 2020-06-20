#pragma once

#include <SDL2/SDL.h>

class Game {
private:
  SDL_Window* m_Window;
  bool m_IsRunning;
  void ProcessInput();
  void UpdateGame();
  void GenerateOutput();
public:
  Game();
  ~Game();
  bool Initialize();
  void RunLoop();
  void ShutDown();
};
