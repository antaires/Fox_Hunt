#pragma once

#include "Math.h"

#include <vector>
#include <string>

// used to prevent actors from passing through barriers
class Map
{
private:
  int m_Rows;
  int m_Cols;
  std::vector<int> csv;

  bool LoadCsv(std::string fileName);
public:
  Map(std::string fileName);
  bool CollidesWithBarrier(Vector2 position);

  std::vector<int> GetCsv() const;
};
