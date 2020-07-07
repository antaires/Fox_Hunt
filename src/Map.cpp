#include "Map.h"
#include "Constants.h"
#include "CollisionDetection.h"
#include "RectangleComponent.h"
#include "Math.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <queue>

Map::Map(std::string fileName)
  : m_Rows(0)
  , m_Cols(0)
  , m_CellWidth(0)
  , m_CellHeight(0)
{
  LoadCsv(fileName);
  BuildGraph();
}

bool Map::LoadCsv(std::string fileName)
{
  std::ifstream fin(fileName, std::ios::in);

  if (!fin)
  {
    std::cout<<"\nWorld map failed to load m_Csv file: " << fileName;
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
      m_Csv.push_back(stoi(word));
    }
    if (!haveCountedCols) {haveCountedCols = true;}
  }

  fin.close();

  m_CellWidth = SCREEN_WIDTH / m_Cols;
  m_CellHeight = SCREEN_HEIGHT / m_Rows;

  return true;
}

void Map::BuildGraph()
{
  // make an unordered_map <m_Csv int, graphnode*> to first build all graph nodes
  // and cal position (using only non-barrier nodes)
  // then, use this map to quickly set all adjacent nodes for each node by checking
  // for top, left, bottom, right nodes in unordered_map. if not there, its a barrer.
  std::unordered_map<int, GraphNode*> csvToNode; // TODO store this?
  for(int i = 0; i < m_Csv.size(); ++i)
  {
    // only add if not barrier
    if (m_Csv.at(i) < 1)
    {
      GraphNode* gn = new GraphNode();
      gn->m_Index = i; // TODO need to store this?
      gn->m_Position = GetPositionFromCsvIndexCentered(i);
      csvToNode[i] = gn;
    }
  }

  // TODO loop over csvToNode and add adjacencies if present in map
  for(auto iter = csvToNode.begin(); iter!=csvToNode.end(); ++iter)
  {
    int index = iter->first;
    GraphNode* gn = iter->second;
    // check 4 directions, if indices in unordered_map, then add to adjacencies
    std::vector<int> directions;
    directions.push_back(index - m_Cols);
    directions.push_back(index + m_Cols);
    directions.push_back(index - 1);
    directions.push_back(index + 1);

    for(auto dir : directions)
    {
      auto it = csvToNode.find(dir);
      if (it != csvToNode.end())
      {
        // node is a walkway, add to adjacencies
        gn->m_Adjacent.push_back(it->second);
      }
    }

    // add node to graph
    m_Graph.m_Nodes.push_back(gn);
  }
}

bool Map::CollidesWithBarrier(Vector2 pos, float width, float height)
{
  // checks all cells that are colliding with actor for barriers

  // convert screen position to 1d array index
  int centerIndex = ConvertPositionto1Dindex(pos);

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
    if (index < m_Csv.size())
    {
      int cell = m_Csv.at(index);
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
      if ( neighborCell < m_Csv.size() && it == visited.end())
      {
        // if collides with actor, add to toCheck
        /*
        int y = std::floor(neighborCell / m_Cols);
        int x = neighborCell % m_Cols;
        Vector2 neighborPos( x * m_CellWidth, y * m_CellHeight );
        */
        Vector2 neighborPos = GetPositionFromCsvIndex(neighborCell);
        if (CollisionDetection::HasCollision(Vector2(pos.x - width/2, pos.y - height/2), width, height, neighborPos, m_CellWidth, m_CellHeight))
        {
          toCheck.push_back(neighborCell);
        }
      }
    }
  }

  return false;
}

Vector2 Map::GetPositionFromCsvIndex(int index)
{
  int y = std::floor(index / m_Cols);
  int x = index % m_Cols;
  return Vector2( x * m_CellWidth, y * m_CellHeight );
}

int Map::ConvertPositionto1Dindex(Vector2 pos)
{
  int i = (int) pos.y / m_CellHeight;
  int j = (int) pos.x / m_CellWidth;
  return m_Cols * i + j;
}

Vector2 Map::GetPositionFromCsvIndexCentered(int index)
{
  int y = std::floor(index / m_Cols);
  int x = index % m_Cols;
  return Vector2( (x * m_CellWidth) + m_CellWidth/2, (y * m_CellHeight) + m_CellHeight/2 );
}

std::vector<int> Map::GetCsv() const
{
  // to be used by AI and pathfinding to build graph
  return m_Csv;
}

std::vector<Vector2> Map::GetPath(Vector2 from, Vector2 to)
{
  std::vector<Vector2> path;

  // calculate index from position
  int indexFrom = ConvertPositionto1Dindex(from);
  int indexTo   = ConvertPositionto1Dindex(to);

  // BFS
  // search from to to from, to avoid reversing path
  NodeToParentMap parentMap;
  bool found = BFS(m_Graph, m_Graph.m_Nodes[indexTo], m_Graph.m_Nodes[indexFrom], parentMap);
  if(found)
  {
    // TODO reconstruct path by using parent pointers in outMap
  }

  // GBFS

  // A*
  return path;
}

bool Map::BFS(const Graph& graph, const GraphNode* start, const GraphNode* goal, NodeToParentMap& outMap)
{
  // whether we found a path
  bool pathFound = false;

  // nodes to consider
  std::queue<const GraphNode*> queue;
  queue.emplace(start);

  while(!queue.empty())
  {
    // dequeue a node
    const GraphNode* current = queue.front();
    queue.pop();
    if (current == goal)
    {
      pathFound = true;
      break;
    }

    // enqueue adjacent nodes that aren't already in queue
    for(const GraphNode* node: current->m_Adjacent)
    {
      // if parent is null, it hasn't been enqueued  (excluding start node)
      const GraphNode* parent = outMap[node];
      if (parent == nullptr && node != start)
      {
        // enqueue this node and set its parent
        outMap[node] = current;
        queue.emplace(node);
      }
    }
  }

  return pathFound;
}

bool Map::GBFS()
{
  // todo
  return false;
}

bool Map::AStar()
{
  return false;
}
