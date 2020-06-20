#include "Game.h"

Game::Game()
{
  m_Window = nullptr;
  m_IsRunning = true;
}

bool Game::Initialize()
{
  // setup SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    SDL_Log("Unable to Initialize SDL: %s", SDL_GetError());
    return false;
  }

  m_Window = SDL_CreateWindow(
    "Fox Hunt"
    , 100   // top left  x-coord
    , 100   // top left  y-coord
    , 1024  // width
    , 768   // height
    , 0     // flags
  );

  if (!m_Window)
  {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  return true;
}

void Game::RunLoop()
{
  while(m_IsRunning)
  {
    ProcessInput();
    UpdateGame();
    GenerateOutput();
  }
}

void Game::ProcessInput()
{
  // TODO
}

void Game::UpdateGame()
{
  // TODO
}

void Game::GenerateOutput()
{
  // TODO
}

void Game::ShutDown()
{
  SDL_DestroyWindow(m_Window);
  SDL_Quit();
}
