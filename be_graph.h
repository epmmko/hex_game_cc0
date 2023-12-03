/* This is for sparse graph */

#ifndef BE_GRAPH_H_
#define BE_GRAPH_H_

#include <unordered_set>
#include <unordered_map>
#include <iostream>

namespace HexBE{

struct NodeDataBE{
 public:
  std::unordered_set<int> mNeighbors;
  std::unordered_set<int> mPossibleNeighbors;
};

class GraphBE{
 public:
  // Initialization
  void Initialize(int board_size);

  // Capacity
  int GetSize() const; // Graph Size (number of vertices)
  
  // Observers
  bool IsExist(int index) const;
  std::unordered_set<int>* GetPossibleNeighbors(int node);
  std::unordered_set<int>* GetNeighbors(int node);
    //return pointer to object instead of the copy of the object to avoid copying
  std::unordered_set<int> FindConnectedVertices(int node_start);
  bool IsInSameSpanningTree(int node_a, int node_b);
  int GetIndex(int row, int col) const;
  void GetRowCol(int index, int &row, int &col) const;

  // Modifiers
  void AddNode(int index);  

  // Printi for Debugging
  std::ostream& PrintGraph(std::ostream& out) const;
  std::ostream& PrintMember(std::ostream& out) const;

 private:
  // Internal data
  int mBoardSize;
  std::unordered_map<int,NodeDataBE> mNodes;

  // Internal Calculation
  std::unordered_set<int> FindNeighbors(int index) const;
  std::unordered_set<int> FindPossibleNeighbors(int index) const;


  // Print for debugging
  template<class T, class V>
  std::ostream& PrintKey(std::ostream& out, std::unordered_map<T,V> map) const;

  template<class T, class V>
  std::ostream& PrintMap(std::ostream& out, std::unordered_map<T,V> map) const;

};

std::ostream& operator<<(std::ostream& out, NodeDataBE node);

template<class T>
std::ostream& operator<<(std::ostream& out, std::unordered_set<T> set);

} // namespace HexBE
#endif // BE_GRAPH_H_
