#include "Map.h"
#include "Constants.h"
#include "CollisionDetection.h"
#include "RectangleComponent.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

Map::Map(std::string fileName)
  : m_Rows(0)
  , m_Cols(0)
{
  LoadCsv(fileName);
}

bool Map::LoadCsv(std::string fileName)
{
  std::ifstream fin(fileName, std::ios::in);

  if (!fin)
  {
    std::cout<<"\nWorld map failed to load csv file: " << fileName;
    return false;
  }

  std::string line, word, temp;
  bool haveCountedCols = false;
  // loop over all rows
  while(std::getline(fin, line))
  {
    m_Rows++; // count rows

    // break line into words
    // read every column data of a row and store it in string var 'word'
    std::stringstream ss(line);
    while(std::getline(ss, word, ','))
    {
      if(!haveCountedCols)
      {
        ++m_Cols;
      }
      // add word to vector as number
      csv.push_back(stoi(word));
    }
    if (!haveCountedCols) {haveCountedCols = true;}
  }

  fin.close();

  return true;
}

bool Map::CollidesWithBarrier(Vector2 pos, float width, float height)
{
  // TODO need to get ALL CELLS that the rectangle overlaps ... and check each

  int i = std::ceil((pos.y * m_Rows) / SCREEN_HEIGHT);
  int j = std::ceil((pos.x * m_Cols) / SCREEN_WIDTH);

  int index = m_Cols * i + j;
  if (index < csv.size())
  {
    int centerCell = csv.at(index);

    // TODO :
    // 1. get center CELL and add to set

    // 2. if has collision, get all CELLS around current cell and add to set

    // 3. once set complete, check for barriers - if any barriers, return true


    if (centerCell == -1 || centerCell == 0)
    {
      // no collision, occupied cell is empty
      return false;
    }

  }

  return true;
}

std::vector<int> Map::GetCsv() const
{
  // to be used by AI and pathfinding to build graph
  return csv;
}
