#pragma once

#include "SpriteComponent.h"
#include <vector>
#include <string>

// reads in a CSV file and draws map using Tile.png
class TileMapComponent: public SpriteComponent {
private:
  int m_CsvRows;
  int m_CsvCols;
  std::vector<int> csv; // TODO choose size of tile map and set size at start
    // TODO once working, switch to 2D vector OR store width by counting chars until first \n char
    // and store height by counting all \n
public:
  TileMapComponent(class Actor* owner, int drawOrder = 10);

  bool LoadCsv(std::string fileName);

  void Draw(SDL_Renderer* renderer) override;
};
