#include <iostream>
#include <vector>
#include "be_hexboard.h"
#include "be_graph.h"

namespace HexBE{

void HexBoardBE::AddP1Move(int move){
  mP1Moves.insert(move);
  ++mPly;
  if(mPly % 2 == 0){
    mCurrentPlayerTurn = EPlayerTurn::EPlayer1;
  } else {
    mCurrentPlayerTurn = EPlayerTurn::EPlayer2;
  }
}

void HexBoardBE::AddP2Move(int move){
  mP2Moves.insert(move);
  ++mPly;
  if(mPly % 2 == 0){
    mCurrentPlayerTurn = EPlayerTurn::EPlayer1;
  } else {
    mCurrentPlayerTurn = EPlayerTurn::EPlayer2;
  }
}

EWinner FindWinner(const std::unordered_set<int> &p1_move,
    const std::unordered_set<int> &p2_move, int board_size,
    std::unordered_set<int> &winner_moves){
  if((p1_move.size() < board_size) && (p2_move.size() < board_size)){
    return EWinner::ENone;
  }

  GraphBE p1_graph;
  GraphBE p2_graph;
  std::vector<GraphBE*> graph_vec = {&p1_graph, &p2_graph};
  std::vector<const std::unordered_set<int>*> move_vec = {&p1_move, &p2_move};

  bool found_start_point;
  bool found_end_point;
  std::unordered_set<int> visited = {};
  std::unordered_set<int> mst_set;

  p1_graph.Initialize(board_size);
  p2_graph.Initialize(board_size);


  for(int i: p1_move){
    p1_graph.AddNode(i);
  }

  for(int i: p2_move){
    p2_graph.AddNode(i);
  }

  //Check if p1 win or not
  for(int i = 0; i <= 1; ++i){
    for(auto it = move_vec[i]->begin(); it != move_vec[i]->end(); ++it){
      if(visited.find(*it) != visited.end()){
        //Skip if already calculated
        continue;
      } else {
        mst_set = graph_vec[i]->FindConnectedVertices(*it);
        visited.insert(mst_set.begin(), mst_set.end());
      } // if visited
      // Reset the previous results
      found_start_point = false;
      found_end_point = false;
      if(i == 0){
        // Player 1 case
        for(int k = 0; k < board_size; ++k){ //check top-left
          if(mst_set.find(k) != mst_set.end()){
            found_start_point = true;
            break;
          }
        }
        for(int k = 0; k < board_size; ++k){ //check bottom-right
          if(mst_set.find((board_size-1)*board_size + k) != mst_set.end()){
            found_end_point = true;
            break;
          }
        }
      } else { // p2 case
        for(int k = 0; k < board_size; ++k){
          if(mst_set.find(k*board_size) != mst_set.end()){
            // bottom left
            found_start_point = true;
            break;
          }
        }
        for(int k = 0; k < board_size; ++k){
          if(mst_set.find(k*board_size + board_size -1) != mst_set.end()){
            // top right
            found_end_point = true;
            break;
          }
        }
      } // if for checkpoint selection
      if(found_start_point && found_end_point){
        winner_moves = mst_set;
        if(i == 0){
          return EWinner::EPlayer1Win;
        } else {
          return EWinner::EPlayer2Win;
        }
      }
    }
  } // graph_vec loop
  return EWinner::ENone;
}

EWinner FindWinnerOfFilledBoard(const std::unordered_set<int> &p1_move,int board_size){

  GraphBE p1_graph;

  bool found_start_point;
  bool found_end_point;
  std::unordered_set<int> visited = {};
  std::unordered_set<int> mst_set;

  p1_graph.Initialize(board_size);

  for(int i: p1_move){
    p1_graph.AddNode(i);
  }

  //Check if p1 win or not
  
  for(auto it = p1_move.begin(); it != p1_move.end(); ++it){
    if(visited.find(*it) != visited.end()){
      //Skip if already calculated
      continue;
    } else {
      mst_set =  p1_graph.FindConnectedVertices(*it);
      visited.insert(mst_set.begin(), mst_set.end());
    } // if visited
    // Reset the previous results
    found_start_point = false;
    found_end_point = false;
    
    // Player 1 case
    for(int k = 0; k < board_size; ++k){ //check top-left
      if(mst_set.find(k) != mst_set.end()){
        found_start_point = true;
        break;
      }
    }
    for(int k = 0; k < board_size; ++k){ //check bottom-right
      if(mst_set.find((board_size-1)*board_size + k) != mst_set.end()){
        found_end_point = true;
        break;
      }
    }
  
    if(found_start_point && found_end_point){
      return EWinner::EPlayer1Win;
    } else {
      return EWinner::EPlayer2Win;
    }
  }
  std::cout << "Error: Cannot find winner in the filled board\n";
  return EWinner::ENone;
}

std::ostream& operator<<(std::ostream& out, EWinner winner){
  switch(winner){
    case(EWinner::EPlayer1Win):{
      out << "Player 1 win\n";
      break;
    }
    case(EWinner::EPlayer2Win):{
      out << "Player 2 win\n";
      break;
    }
    case(EWinner::ENone):{
      out << "None win\n";
      break;
    }
  }
  return out;
}

} // HexBe
