/*
Hex game graph

board_size = 7 (for 7 x 7 board)

Visual representation (cells)
                         6 ___ 0
                     5 ___/0,6\___ 1        
                 4 ___/0,5\___/1,6\___ 2 
             3 ___/0,4\___/1,5\___/2,6\___ 3 
         2 ___/0,3\___/1,4\___/2,5\___/3,6\___ 4 
     1 ___/0,2\___/1,3\___/2,4\___/3,5\___/4,6\___ 5 
 0 ___/0,1\___/1,2\___/2,3\___/3,4\___/4,5\___/5,6\___ 6 
  /0,0\___/1,1\___/2,2\___/3,3\___/4,4\___/5,5\___/6,6\  
  \___/1,0\___/2,1\___/3,2\___/4,3\___/5,4\___/6,5\___/  Line 8
      \___/2,0\___/3,1\___/4,2\___/5,3\___/6,4\___/  Line 9
          \___/3,0\___/4,1\___/5,2\___/6,3\___/  Line 10
              \___/4,0\___/5,1\___/6,2\___/  Line 11
                  \___/5,0\___/6,1\___/  Line 12
                      \___/6,0\___/  Line 13
                          \___/  Line 14

Player 1 graph during the game

                         6 ___ 0
                     5 ___/0,6\___ 1        
                 4 ___/   \_1_/1,6\___ 2 
             3 ___/0,4\_1_/1,5\___/2,6\___ 3 
         2 ___/0,3\___/   \___/2,5\___/3,6\___ 4 
     1 ___/0,2\___/1,3\_1_/2,4\___/   \___/4,6\___ 5 
 0 ___/0,1\___/   \___/   \___/3,4\_1_/   \___/5,6\___ 6 
  /0,0\___/   \_1_/2,2\_1_/   \___/4,4\_1_/5,5\___/6,6\  
  \_1_/   \_1_/2,1\___/3,2\_1_/   \___/5,4\___/6,5\_1_/  Line 8
      \_1_/2,0\___/3,1\___/4,2\_1_/5,3\___/6,4\_1_/  Line 9
          \___/3,0\___/4,1\___/   \___/6,3\___/  Line 10
              \___/4,0\___/5,1\_1_/   \___/  Line 11
                  \___/5,0\___/6,1\_1_/  Line 12
                      \___/6,0\___/  Line 13
                          \_1_/  Line 14


The distance is between node is always 1

*/
#include <iostream>
#include <iterator>
#include <cmath>
#include "be_graph.h"
#include <unordered_set>

namespace HexBE{

//Initialization
void GraphBE::Initialize(int board_size){
  mBoardSize = board_size;
}

//Capacity
int GraphBE::GetSize() const{
  return mNodes.size();
}

//Observers
bool GraphBE::IsExist(int index) const{
  //Determine if the vertex at the input index exists or not.
  return mNodes.find(index) != mNodes.end();
}

std::unordered_set<int>* GraphBE::GetPossibleNeighbors(int node){
  if(mNodes.find(node) != mNodes.end()){
    return &(mNodes.at(node).mPossibleNeighbors);
  } else {
    return nullptr;
  }
}

std::unordered_set<int>* GraphBE::GetNeighbors(int node){
  if(mNodes.find(node) != mNodes.end()){
    return &(mNodes.at(node).mNeighbors);
  } else {
    return nullptr;
  }
}

std::unordered_set<int> GraphBE::FindConnectedVertices(int node_start) {
  // For the efficiency in hex-game, just find the connection, but not need to be a minimum distance.
  // This just tell which vertices are connected to the input vertex
  // CC0: Creative Common License
  // Created by Ekarit Panacharoensawad
  std::unordered_set<int> unvisited;
  std::unordered_set<int> visited;
  std::unordered_set<int> active_neighbor;
  std::unordered_set<int> neighbor_of_start_index;

  
  for(auto it = mNodes.begin(); it != mNodes.end(); ++it){
    unvisited.insert(it->first);
  }

  int start_index = node_start; //current vertex
  while(true){
    visited.insert(start_index);
    unvisited.erase(start_index);
    auto ptr = GetNeighbors(start_index);
    if(ptr != nullptr){
      neighbor_of_start_index = *ptr;
    } else {
      neighbor_of_start_index = std::unordered_set<int> {};
    }
      // room for improvement (later) -> store connection as unordered set.
    for(auto neighbor: neighbor_of_start_index){
      if(visited.find(neighbor) == visited.end()){ 
      // neighbor is not found in the visited list (this is an unvisited neighbor)
        active_neighbor.insert(neighbor);
      }
    }
    if(active_neighbor.size() == 0) break;
    start_index = *active_neighbor.begin();
    active_neighbor.erase(active_neighbor.begin());

  } // End of while loop
  return visited;
}

bool GraphBE::IsInSameSpanningTree(int node_a, int node_b){
  std::unordered_set<int> a_spanning_tree = FindConnectedVertices(node_a);
  return a_spanning_tree.find(node_b) != a_spanning_tree.end();
}

int GraphBE::GetIndex(int row, int col) const{
  return row * mBoardSize + col;
}

void GraphBE::GetRowCol(int graph_index, int &row, int &col) const{
  row = graph_index / mBoardSize;
  col = graph_index % mBoardSize;
}

// Modifiers
void GraphBE::AddNode(int index){
  // First, find all existing neighbors. Specify them as the node data
  // Next, notify the neighbor node of the existence of the current node
  std::unordered_set<int> neighbors = FindNeighbors(index);
  NodeDataBE data = {
    neighbors,
    FindPossibleNeighbors(index)
  };

  //update the connection of the existing neighbor
  if(!neighbors.empty()){
    for(auto i: neighbors){
      mNodes[i].mNeighbors.insert(index);
    }
  }
  mNodes.insert({index,std::move(data)});
}

// Internal Calculation
std::unordered_set<int> GraphBE::FindNeighbors(int index) const{
  //First find all possible neighbors and add them as the connection
  //Then, delete the connection if there is no vertex at the available location
  std::unordered_set<int> SixNeighborIndex = FindPossibleNeighbors(index);

  // Check if there is any neighbor on the possible neighbor location
  for(auto it = SixNeighborIndex.begin(); it != SixNeighborIndex.end(); ){
    if(!IsExist(*it)){
      it = SixNeighborIndex.erase(it);
        //The reterned iterator is the next iterator after the deleted member
    } else {
      ++it;
    }
  }
  return SixNeighborIndex;
}

std::unordered_set<int> GraphBE::FindPossibleNeighbors(int index) const{
  // Possible neighbor are all cells connected to the input index location
  // Except
  //   1 cells that are outside the board in
  std::unordered_set<int> SixNeighborIndex;
  SixNeighborIndex.insert(index - 1);
  SixNeighborIndex.insert(index + 1);
  SixNeighborIndex.insert(index + mBoardSize); //down-right
  SixNeighborIndex.insert(index + mBoardSize - 1); //down
  SixNeighborIndex.insert(index - mBoardSize + 1); //up
  SixNeighborIndex.insert(index - mBoardSize); //up-left

  int index_row, index_col; //row and col of the index
  GetRowCol(index, index_row, index_col);
  int row, col;

  // Check if the neighbor's index is outside the possible row & col ranges or not
  for(auto it = SixNeighborIndex.begin(); it != SixNeighborIndex.end(); ){
    GetRowCol(*it, row, col);
    if((*it < 0) || (*it > mBoardSize * mBoardSize - 1)){
      it = SixNeighborIndex.erase(it);
    } else if((std::abs(row - index_row) > 1) || (std::abs(col - index_col) > 1)){
      it = SixNeighborIndex.erase(it);
    } else {
      ++it;
    }
  }

  return SixNeighborIndex;
}

// Print for Debugging
std::ostream& GraphBE::PrintMember(std::ostream& out) const{
  PrintKey(out, mNodes);
  return out;
}

std::ostream& GraphBE::PrintGraph(std::ostream& out) const{
  PrintMap(out, mNodes);
  return out;
}

template<class T, class V>
std::ostream& GraphBE::PrintKey(std::ostream& out, std::unordered_map<T,V> map) const{
  out << "{";
  for(auto it = map.begin(); it != map.end(); ++it){
    out << it->first;
    if(std::next(it) != map.end()){
      out <<", ";
    }
  }
  out <<"}";
  return out;
}

template<class T, class V>
std::ostream& GraphBE::PrintMap(std::ostream& out, std::unordered_map<T,V> map) const{
  out << "{";
  for(auto it = map.begin(); it != map.end(); ++it){
    out << it->first << ": " << it->second;
    if(std::next(it) != map.end()){
      out <<", ";
    }
  }
  out <<"}";
  return out;
}

template<class T>
std::ostream& operator<<(std::ostream& out, std::unordered_set<T> set){
  out << "{";
  for(auto it = set.begin(); it != set.end(); ++it){
    out << *it;
    if(std::next(it) != set.end()){
      out << ", ";
    }
  }
  out << "}";
  return out;
}

std::ostream& operator<<(std::ostream& out, NodeDataBE node){
  out << "{Neighbor: " << node.mNeighbors << ", PossibleNeighbor: " << node.mPossibleNeighbors << "}\n";
  return out;
}

} // namespace HexBE
