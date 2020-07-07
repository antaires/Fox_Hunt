#pragma once

#include "Math.h"

#include <vector>
#include <string>

struct GraphNode
{
  // pointers to adjacent nodes
  int m_Index; // stores index for csv vector
  Vector2 m_Position;
  std::vector<GraphNode*> m_Adjacent;
};

struct Graph
{
  std::vector<GraphNode*> m_Nodes;
};

// used to prevent actors from passing through barriers
class Map
{
private:
  int m_Rows;
  int m_Cols;
  int m_CellWidth;
  int m_CellHeight;
  std::vector<int> m_Csv;

  Graph m_Graph;

  bool LoadCsv(std::string fileName);
  void BuildGraph();
  Vector2 GetPositionFromCsvIndex(int index);
  Vector2 GetPositionFromCsvIndexCentered(int index);
public:
  Map(std::string fileName);
  bool CollidesWithBarrier(Vector2 pos, float width, float height);
  std::vector<int> GetCsv() const;

  // std::vector<Vector2> AStar(Vector2 from, Vector2 to);
};
