#include "Game.h"
#include "Constants.h"

Game::Game()
{
  m_Window = nullptr;
  m_IsRunning = true;

  // TODO : temporary
  m_FoxPos.x = SCREEN_WIDTH/2; m_FoxPos.y = SCREEN_HEIGHT/2;
  m_HunterPos.x = 0; m_HunterPos.y = 0;
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
    , SCREEN_WIDTH  // width
    , SCREEN_HEIGHT   // height
    , 0     // flags
  );

  if (!m_Window)
  {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  m_Renderer = SDL_CreateRenderer(
    m_Window
    , -1
    , SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );

  if (!m_Renderer)
  {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
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
  SDL_SetRenderDrawColor(
    m_Renderer
    , 0
    , 0
    , 255
    , 255
  );

  SDL_RenderClear(m_Renderer);

  DrawGameScene();

  SDL_RenderPresent(m_Renderer);
}

void Game::DrawGameScene()
{
  // draw fox
  SDL_SetRenderDrawColor(m_Renderer, 255, 136, 0, 255); // bright orange
  SDL_Rect foxRect {
      static_cast<int> (m_FoxPos.x - FOX_WIDTH / 2)
    , static_cast<int> (m_FoxPos.y - FOX_HEIGHT / 2)
    , FOX_WIDTH
    , FOX_HEIGHT
  };
  SDL_RenderFillRect(m_Renderer, &foxRect);

  // draw hunter
  SDL_SetRenderDrawColor(m_Renderer, 255, 21, 0, 255); // bright red
  SDL_Rect hunterRect {
      static_cast<int> (m_HunterPos.x - HUNTER_WIDTH / 2)
    , static_cast<int> (m_HunterPos.y - HUNTER_HEIGHT / 2)
    , HUNTER_WIDTH
    , HUNTER_HEIGHT
  };
  SDL_RenderFillRect(m_Renderer, &hunterRect);
}

void Game::ShutDown()
{
  SDL_DestroyWindow(m_Window);
  SDL_DestroyRenderer(m_Renderer);
  SDL_Quit();
}
