#pragma once

#include "Math.h"

#include <unordered_map>
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

using NodeToParentMap = std::unordered_map<const GraphNode*, const GraphNode*>;

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
  int ConvertPositionto1Dindex(Vector2 pos);

  bool BFS(const Graph& graph, const GraphNode* start, const GraphNode* goal, NodeToParentMap& outMap);
  bool GBFS();
  bool AStar();

public:
  Map(std::string fileName);
  bool CollidesWithBarrier(Vector2 pos, float width, float height);
  std::vector<int> GetCsv() const;

  std::vector<Vector2> GetPath(Vector2 from, Vector2 to);
};
