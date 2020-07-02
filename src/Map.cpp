#include "Map.h"
#include "Constants.h"

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

bool Map::CollidesWithBarrier(Vector2 position)
{
  // todo check
  int i = std::ceil((position.y * m_Rows) / SCREEN_HEIGHT);
  int j = std::ceil((position.x * m_Cols) / SCREEN_WIDTH);

  int index = m_Cols * i + j;
  if (index < csv.size())
  {
    int cell = csv.at(index);

    std::cout<<"\n x:"<<position.x<<" y:"<<position.y<<" i:"<<i<<" j:"<<j<<" cell: "<<cell;

    if (cell == -1 || cell == 0)
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
