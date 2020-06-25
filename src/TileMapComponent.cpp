#include "TileMapComponent.h"
#include <iostream>
#include <fstream>
#include <sstream>

TileMapComponent::TileMapComponent(class Actor* owner, int drawOrder)
  :SpriteComponent(owner, drawOrder)
  , m_CsvRows(0)
  , m_CsvCols(0)
{}

bool TileMapComponent::LoadCsv(std::string fileName)
{
  std::ifstream fin("assets/mapLayer01.csv", std::ios::in);

  std::string line, word, temp;

  // loop over all rows
  while(std::getline(fin, line))
  {
    m_CsvRows++; // count rows

    // break line into words
    // read every column data of a row and store it in string var 'word'
    std::stringstream ss(line);
    while(std::getline(ss, word, ',')) // TODO: breaks if spaces like ", " ?
    {
      // TODO count columns
      // add word to vector as number
      csv.push_back(stoi(word));
    }
  }

  // TODO: testing
  for(int i : csv)
  {
    std::cout<< i << " ";
  }

  fin.close();
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
  if (m_Texture)
  {
    for(int i : csv)
    {
      /*
      // TODO : get location of current i in tilemap texture
      SDL_Rect source;
      source.w = static_cast<int>(m_TextureWidth);
      source.h = static_cast<int>(m_TextureHeight);
      source.x = static_cast<int>(0);
      source.y = static_cast<int>(0);

      // TODO: get location of current draw rect on screen
      SDL_Rect tile;
      tile.w = static_cast<int>(m_TextureWidth * m_Owner->GetScale());
      tile.h = static_cast<int>(m_TextureHeight * m_Owner->GetScale());
      tile.x = static_cast<int>(m_Owner->GetPosition().x - r.w / 2);
      tile.y = static_cast<int>(m_Owner->GetPosition().y - r.h / 2);

      SDL_RenderCopyEx(
         renderer
        , m_Texture
        , &source
        , &destination
        , 0 // TODO: add rotation info to sprite sheet?
        , nullptr
        , SDL_FLIP_NONE
      );
      */
    }
  }
}
