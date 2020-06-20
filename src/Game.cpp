#include "Game.h"
#include "Constants.h"
#include "Component.h"
#include "SpriteComponent.h"

Game::Game()
{
  m_Window = nullptr;
  m_IsRunning = true;
  m_TicksCount = 0;

  // TODO : temporary
  m_Fox.pos.x = SCREEN_WIDTH/2; m_Fox.pos.y = SCREEN_HEIGHT/2;
  m_Hunter.pos.x = 0; m_Hunter.pos.y = 0;
  m_Fox.vel.x = 0; m_Fox.vel.y = 0;
  m_Hunter.vel.x = 0; m_Hunter.vel.y = 0;
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

  IMG_init(IMG_INIT_PNG | IMG_INIT_JPG); // can add flags for different types

  LoadData();

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
    m_Fox.vel.y -= 1;
  }
  if (state[SDL_SCANCODE_S])
  {
    m_Fox.vel.y += 1;
  }
  if (state[SDL_SCANCODE_A])
  {
    m_Fox.vel.x -= 1;
  }
  if (state[SDL_SCANCODE_D])
  {
    m_Fox.vel.x += 1;
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
  m_UpdatingActors = true;
  for(auto actor: m_Actors)
  {
    actor->Update(deltaTime);
  }
  m_UpdatingActors = false;

  // move pending actors to actors and clear
  for(auto pending : m_PendingActors)
  {
    m_Actors.emplace_back(pending);
  }
  m_PendingActors.clear();

  // add dead actors to temp vector
  std::vector<Actor*> deadActors;
  for(auto actor : m_Actors)
  {
    if (actor->GetState() == Actor::E_Dead)
    {
      deadActors.emplace_back(actor);
    }
  }

  // delete dead actors
  for(auto actor : deadActors)
  {
    delete actor; // TODO is this the safe way?
  }

  // update fox pos
  if (m_Fox.vel.y != 0)
  {
    m_Fox.pos.y += m_Fox.vel.y * FOX_SPEED * deltaTime;
    ClampToScreen(m_Fox.pos.y, FOX_HEIGHT, SCREEN_HEIGHT);
  }
  if (m_Fox.vel.x != 0)
  {
    m_Fox.pos.x += m_Fox.vel.x * FOX_SPEED * deltaTime;
    ClampToScreen(m_Fox.pos.x, FOX_WIDTH, SCREEN_WIDTH);
  }

  // TODO update hunter pos based on fox position

}

void Game::ClampToScreen(float& pos, int objHeight, int limit)
{
  if (pos < objHeight/2.0f){pos = objHeight/2.0f;}
  if (pos > limit - (objHeight / 2)){pos = limit - (objHeight/2);}
}

void Game::AddActor(Actor* actor)
{
  if (m_UpdatingActors)
  {
    m_PendingActors.emplace_back(actor);
  } else {
    m_Actors.emplace_back(actor);
  }
}

void Game::RemoveActor(Actor* actor)
{
  auto it = std::find(m_Actors.begin(), m_Actors.end(), actor);
  if (it != m_Actors.end())
  {
    m_Actors.erase(it);
  } else {
    it = std::find(m_PendingActors.begin(), m_PendingActors.end(), actor);
    if (it != m_PendingActors.end())
    {
      m_PendingActors.erase(it);
    }
  }
}

SDL_Texture* Game::LoadTexture(const char* fileName)
{
  SDL_Surface* surface = IMG_Load(fileName);
  if (!surface)
  {
    SDL_Log("Failed to load texture file: %s", filename);
    return nullptr;
  }
  SDL_Texture* texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
  SDL_FreeSurface(surface);
  if(!texture)
  {
    SDL_Log("Failed to create texture from surface for: %s", filename);
    return nullptr;
  }
  return texture;
}

// hard coded for now, TODO: load from files and binary
void Game::LoadData()
{
  // TODO : load all textures
}

void Game::AddSprite(SpriteComponent* sprite)
{
  int myDrawOrder = sprite->GetDrawOrder();
  auto it = m_Sprites.begin();
  for(; it != m_Sprites.end(); ++it)
  {
    if (myDrawOrder < (*it)->GetDrawOrder())
    {
      break;
    }
  }

  m_Sprites.insert(it, sprite);
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
      static_cast<int> (m_Fox.pos.x - FOX_WIDTH / 2)
    , static_cast<int> (m_Fox.pos.y - FOX_HEIGHT / 2)
    , FOX_WIDTH
    , FOX_HEIGHT
  };
  SDL_RenderFillRect(m_Renderer, &foxRect);

  // draw hunter
  SDL_SetRenderDrawColor(m_Renderer, 255, 21, 0, 255); // bright red
  SDL_Rect hunterRect {
      static_cast<int> (m_Hunter.pos.x - HUNTER_WIDTH / 2)
    , static_cast<int> (m_Hunter.pos.y - HUNTER_HEIGHT / 2)
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

  while (!m_Actors.empty())
  {
    delete m_Actors.back();
  }
}
