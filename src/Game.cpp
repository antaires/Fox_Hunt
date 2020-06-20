#include "Game.h"
#include "Constants.h"

Game::Game()
{
  m_Window = nullptr;
  m_IsRunning = true;
  m_TicksCount = 0;

  // TODO : temporary
  m_FoxPos.x = SCREEN_WIDTH/2; m_FoxPos.y = SCREEN_HEIGHT/2;
  m_HunterPos.x = 0; m_HunterPos.y = 0;
  m_FoxVel.x = 0; m_FoxVel.y = 0;
  m_HunterVel.x = 0; m_HunterVel.y = 0;
  m_WallPos.x = SCREEN_WIDTH/4;
  m_WallPos.y = SCREEN_HEIGHT/4;
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

  // handle player input
  if (state[SDL_SCANCODE_W])
  {
    m_FoxVel.y -= 1;
  }
  if (state[SDL_SCANCODE_S])
  {
    m_FoxVel.y += 1;
  }
  if (state[SDL_SCANCODE_A])
  {
    m_FoxVel.x -= 1;
  }
  if (state[SDL_SCANCODE_D])
  {
    m_FoxVel.x += 1;
  }


}

void Game::UpdateGame()
{
  // frame limiting : wait until 16 ms elapsed
  while(!SDL_TICKS_PASSED(SDL_GetTicks(), m_TicksCount + 16));

  // deltaTime is difference in ticks from last frame
  float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;
  m_TicksCount = SDL_GetTicks();

  // clamp max delta time value (to avoid jumping ahead during debug)
  if (deltaTime > 0.05f)
  {
    deltaTime = 0.05f;
  }

  // TODO: update objects in game world as function of delta time
  // update fox pos
  if (m_FoxVel.y != 0)
  {
    m_FoxPos.y += m_FoxVel.y * FOX_SPEED * deltaTime;
    ClampToScreen(m_FoxPos.y, FOX_HEIGHT, SCREEN_HEIGHT);
  }
  if (m_FoxVel.x != 0)
  {
    m_FoxPos.x += m_FoxVel.x * FOX_SPEED * deltaTime;
    ClampToScreen(m_FoxPos.x, FOX_WIDTH, SCREEN_WIDTH);
  }
}

void Game::ClampToScreen(float& pos, int objHeight, int limit)
{
  if (pos < objHeight/2.0f){pos = objHeight/2.0f;}
  if (pos > limit - (objHeight / 2)){pos = limit - (objHeight/2);}
}

void Game::GenerateOutput()
{
  SDL_SetRenderDrawColor(
    m_Renderer
    , 38
    , 51
    , 38
    , 255
  );

  SDL_RenderClear(m_Renderer);

  DrawGameScene();

  SDL_RenderPresent(m_Renderer);
}

void Game::DrawGameScene()
{
  // draw wall
  SDL_SetRenderDrawColor(m_Renderer, 59, 128, 59, 255); // bright orange
  SDL_Rect wallRect {
      static_cast<int> (m_WallPos.x - 150 / 2)
    , static_cast<int> (m_WallPos.y - 300 / 2)
    , 150
    , 300
  };
  SDL_RenderFillRect(m_Renderer, &wallRect);

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
