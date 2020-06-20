#include "Game.h"

Game::Game()
{
  m_Window = nullptr;
  m_IsRunning = true;
}

Game::~Game()
{
  SDL_Quit();
}

bool Game::Initialize()
{
  // setup SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("Unable to Initialize SDL: %s", SDL_GetError());
    return false;
  }
  return true;
}

void Game::RunLoop()
{

}
