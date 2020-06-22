#include "Game.h"
#include "SDL2/SDL_image.h"
#include "Constants.h"
#include "SpriteComponent.h"

#include <algorithm>

Game::Game()
  : m_Window(nullptr)
  , m_Renderer(nullptr)
  , m_IsRunning(true)
  , m_UpdatingActors(false)
  , m_TicksCount(0)
  , m_Player(nullptr)
{}

bool Game::Initialize()
{
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

  if (IMG_Init(IMG_INIT_PNG == 0)) // can add flags for different types
  {
    SDL_Log("Unable to init SDL_image: %s", SDL_GetError());
    return false;
  }

  LoadData();
  m_TicksCount = SDL_GetTicks();
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
  m_Player->ProcessKeyboard(state);
}

void Game::UpdateGame()
{
  // frame limiting : wait until 16 ms elapsed
  while(!SDL_TICKS_PASSED(SDL_GetTicks(), m_TicksCount + 16));

  // deltaTime is difference in ticks from last frame
  float deltaTime = (SDL_GetTicks() - m_TicksCount) / 1000.0f;

  // clamp max delta time value (to avoid jumping ahead during debug)
  if (deltaTime > 0.05f)
  {
    deltaTime = 0.05f;
  }
  m_TicksCount = SDL_GetTicks();

  // update objects in game world as function of delta time
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
  // check both pending and active actors
  auto it = std::find(m_PendingActors.begin(), m_PendingActors.end(), actor);
  if (it != m_PendingActors.end())
  {
    // swap to end of vector and pop off (avoid erase copies)
    std::iter_swap(it, m_PendingActors.end() -1);
    m_PendingActors.pop_back();
  }

  it = std::find(m_Actors.begin(), m_Actors.end(), actor);
  if (it != m_Actors.end())
  {
    std::iter_swap(it, m_Actors.end() -1);
    m_Actors.pop_back();
  }
}

// hard coded for now, TODO: load from files and binary
void Game::LoadData()
{
  // TODO : load all textures

  // create player
  m_Player = new Player(this);
  m_Player->SetPosition(Vector2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
  m_Player->SetScale(1.5f);

  // todo create background actor 

  m_WallPos.x = SCREEN_WIDTH/4;
  m_WallPos.y = SCREEN_HEIGHT/4;

  // create background...
}

void Game::UnloadData()
{
  while(!m_Actors.empty())
  {
    delete m_Actors.back();
  }

  for(auto t : m_Textures)
  {
    SDL_DestroyTexture(t.second);
  }
  m_Textures.clear();
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
  SDL_Texture* texture = nullptr;

  // is texture already loaded?
  auto it = m_Textures.find(fileName);
  if (it != m_Textures.end())
  {
    texture = it->second;
  } else {
    // load from file
    SDL_Surface* surface = IMG_Load(fileName.c_str());
    if (!surface)
    {
      SDL_Log("Failed to load texture file: %s", fileName.c_str());
      return nullptr;
    }
    texture = SDL_CreateTextureFromSurface(m_Renderer, surface);
    SDL_FreeSurface(surface);
    if(!texture)
    {
      SDL_Log("Failed to create texture from surface for: %s", fileName.c_str());
      return nullptr;
    }
    m_Textures.emplace(fileName, texture);
  }
  return texture;
}


void Game::AddSprite(SpriteComponent* sprite)
{
  // insert by sorted order
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

void Game::RemoveSprite(SpriteComponent* sprite)
{
  // cant use swap here because need to preserve ordering
  auto it = std::find(m_Sprites.begin(), m_Sprites.end(), sprite);
  if (it != m_Sprites.end())
  {
    m_Sprites.erase(it);
  }
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

  DrawGameScene(); // TODO needed?
  // Draw all sprite components
  for(auto sprite: m_Sprites)
  {
    sprite->Draw(m_Renderer);
  }

  SDL_RenderPresent(m_Renderer);
}

void Game::DrawGameScene()
{
  /*  TODO
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
  */
}

void Game::ShutDown()
{
  UnloadData();
  IMG_Quit();
  SDL_DestroyRenderer(m_Renderer);
  SDL_DestroyWindow(m_Window);
  SDL_Quit();
}
