#include "Map.h"
#include "Constants.h"
#include "CollisionDetection.h"
#include "RectangleComponent.h"
#include "Math.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <unordered_map>

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
  // checks all cells that are colliding with actor for barriers

  // convert screen position to 1d array index
  int i = std::ceil((pos.y * m_Rows) / SCREEN_HEIGHT);
  int j = std::ceil((pos.x * m_Cols) / SCREEN_WIDTH);
  int centerIndex = m_Cols * i + j;

  int cellWidth = SCREEN_WIDTH / m_Cols;
  int cellHeight = SCREEN_HEIGHT / m_Rows;

  // 1. get center CELL and add to set
  std::vector<int> toCheck;
  std::unordered_map<int, int> visited;
  visited[centerIndex] = centerIndex;
  toCheck.push_back(centerIndex);

  // 2. if has collision, get all CELLS around current cell and add to set
  while(!toCheck.empty())
  {
    int index = toCheck.back();
    toCheck.pop_back();
    visited[index] = index;

    // check for collision with barrier
    if (index < csv.size())
    {
      int cell = csv.at(index);
      if (cell >= 1)
      {
        return true; // collision !
      }
    }

    // add neighboring cells that aren't in visited if they collide with actor
    std::vector<int> surroundingCells;
    surroundingCells.push_back(index - 1);
    surroundingCells.push_back(index + 1);
    surroundingCells.push_back(index - m_Cols - 1);
    surroundingCells.push_back(index - m_Cols);
    surroundingCells.push_back(index - m_Cols + 1);
    surroundingCells.push_back(index + m_Cols - 1);
    surroundingCells.push_back(index + m_Cols);
    surroundingCells.push_back(index + m_Cols + 1);

    for(int neighborCell : surroundingCells)
    {
      auto it = visited.find(neighborCell);
      if ( neighborCell < csv.size() && it == visited.end())
      {
        // if collides with actor, add to toCheck
        int x = std::floor(neighborCell / m_Cols);
        int y = neighborCell % m_Cols;
        Vector2 neighborPos( x * cellWidth, y * cellHeight );
        if (CollisionDetection::HasCollision(pos, width, height, neighborPos, cellWidth, cellHeight))
        {
          toCheck.push_back(neighborCell);
        }
      }
    }
  }

  return false;
}

std::vector<int> Map::GetCsv() const
{
  // to be used by AI and pathfinding to build graph
  return csv;
}
