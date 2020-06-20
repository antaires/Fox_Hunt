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
  SDL_Event event;

  // loop over events in event queue
  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      // handle all event types
      case SDL_QUIT:
        m_IsRunning = false;
        break;
    }
  }

  // get state of keyboard
  const Uint8* state = SDL_GetKeyboardState(NULL);

  // if escape pressed, exit Game RunLoop
  if (state[SDL_SCANCODE_ESCAPE])
  {
    m_IsRunning = false;
  }
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
