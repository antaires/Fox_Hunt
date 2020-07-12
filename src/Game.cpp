#include "Game.h"
#include "SDL2/SDL_image.h"
#include "Constants.h"
#include "SpriteComponent.h"
#include "BackgroundSpriteComponent.h"
#include "CollisionDetection.h"
#include "TileMapComponent.h"
#include "Map.h"

#include <algorithm>

#include <iostream> // remove

Game::Game()
  : m_Window(nullptr)
  , m_Renderer(nullptr)
  , m_IsRunning(true)
  , m_UpdatingActors(false)
  , m_TicksCount(0)
  , m_Map(nullptr)
  , m_Player(nullptr)
  // TODO consider setting enemy vector initial size ...
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
    HandleCollisions();
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
  const Uint8* keyState = SDL_GetKeyboardState(NULL);

  // if escape pressed, exit Game RunLoop
  if (keyState[SDL_SCANCODE_ESCAPE])
  {
    m_IsRunning = false;
  }

  // get mouse state
  int mouseX, mouseY;
  const uint32_t mouseState = SDL_GetMouseState(&mouseX, &mouseY);

  // handle input
  m_UpdatingActors = true;
  for(auto actor: m_Actors)
  {
    actor->ProcessInput(keyState);
    actor->ProcessMouse(mouseState, mouseX, mouseY);
  }
  m_UpdatingActors = false;
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
    // TODO delete properly from actors
    auto it = std::find(m_Actors.begin(), m_Actors.end(), actor);
    if (it != m_Actors.end())
    {
      m_Actors.erase(it);
    }
  }
}

void Game::HandleCollisions()
{
  // player against enemy (ies)
  // check if collide with Player - if yes then kill player
  if(m_Player != nullptr && m_Player->GetState() == Actor::E_Active)
  {
    for(auto enemy : m_Enemies)
    {
      if (enemy != nullptr && enemy->GetState() == Actor::E_Active)
      {
        if( CollisionDetection::HasCollision(enemy->GetCircle(), m_Player->GetCircle()) )
        {
          m_Player->SetState(Actor::E_Dead);
        }
      }
    }
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
  // load all textures

  // create path map (barriers and walls) and AI
  m_Map = new Map("assets/testMap01.csv");
  m_Map->SetMode(Map::E_BFS); // choose pathfinding algorithm

  // create player
  m_Player = new Player(this); // scale set inside class
  m_Player->SetPosition(Vector2(SCREEN_WIDTH/2, SCREEN_HEIGHT/3));

  // create initial enemies TODO: make several
  Enemy* enemy1 = new Enemy(this, m_Map); // scale set inside class
  enemy1->SetPosition(Vector2(30, SCREEN_HEIGHT/2));
  m_Enemies.push_back(enemy1);
  Enemy* enemy2 = new Enemy(this, m_Map); // scale set inside class
  enemy2->SetPosition(Vector2(SCREEN_WIDTH - 30, SCREEN_HEIGHT/2));
  m_Enemies.push_back(enemy2);



  // create background tile map
  Actor* tileMapActor = new Actor(this);
  tileMapActor->SetPosition(Vector2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
  TileMapComponent* tileMapComponent = new TileMapComponent(tileMapActor, 10);
  // load CSV
  tileMapComponent->LoadCsv("assets/testMap01.csv");
  // set texture
  tileMapComponent->SetTextureRowsCols( 24, 8);
  tileMapComponent->SetTexture(GetTexture("assets/tiles.png"));

  // create background actor for scrolling fog
  Actor* temp = new Actor(this);
  temp->SetPosition(Vector2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
  BackgroundSpriteComponent* bg = new BackgroundSpriteComponent(temp, 200);
  bg->SetScreenSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
  std::vector<SDL_Texture*> bgTexs = {
    GetTexture("assets/fog01.png")
    , GetTexture("assets/fog01.png")
  };
  bg->SetBGTextures(bgTexs);
  bg->SetScrollSpeed(-50.0f);
  // fog 2
  bg = new BackgroundSpriteComponent(temp);
  bg->SetScreenSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
  bgTexs = {
      GetTexture("assets/fog01.png")
    , GetTexture("assets/fog01.png")
  };
  bg->SetBGTextures(bgTexs);
  bg->SetScrollSpeed(-70.0f);
  // if want scrolling and paralax, create more bg's here to attach to temp actor

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

  // Draw all sprite components
  for(auto sprite: m_Sprites)
  {
    sprite->Draw(m_Renderer);
  }

  SDL_RenderPresent(m_Renderer);
}

Vector2 Game::GetPlayerPosition() const
{
  return m_Player->GetPosition();
}

class Player* Game::GetPlayer()
{
  return m_Player;
}


std::vector<Enemy*> Game::GetEnemies()
{
  return m_Enemies;
}

bool Game::CollidesWithBarrier(Vector2 pos, float width, float height)
{
  return m_Map->CollidesWithBarrier(pos, width, height);
}

void Game::ShutDown()
{
  UnloadData();
  IMG_Quit();
  SDL_DestroyRenderer(m_Renderer);
  SDL_DestroyWindow(m_Window);
  SDL_Quit();
}
