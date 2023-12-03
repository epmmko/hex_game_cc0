#include <chrono>
#include <unordered_set>
#include <random> 				// std::default_random_engine
#include <algorithm>    	// std::shuffle, std::generate_n, std::sort
#include <iterator>				// std::inserter
#include <vector>
#include <utility>				// std::pair
#include "be_hex_ai.h"
#include "be_hexboard.h"	// EPlayerTurn

//for debugging
#include <iostream>

namespace HexBE{

void MonteCarloAI::Initialize(int board_size, int trial_times){
  mBoardSize = board_size;
  mTrialTimes = trial_times;
  mSeed = std::chrono::system_clock::now().time_since_epoch().count();
}

int MonteCarloAI::GetAiMove(const std::unordered_set<int> &p1_moves,
    const std::unordered_set<int> &p2_moves){
  std::unordered_set<int> available_moves;
  std::unordered_set<int> p1_moves_to_be_filled;
  std::unordered_set<int> p2_moves_to_be_filled;
    // First, before each fill, these set will be reset to the
      // input moves
  EPlayerTurn player_turn;

  if((p1_moves.size() + p2_moves.size()) % 2 == 0){
    player_turn = EPlayerTurn::EPlayer1;
  } else {
    player_turn = EPlayerTurn::EPlayer2;
  }

  // Constructing available_moves variable
  for(int i = 0; i < mBoardSize * mBoardSize; ++i){
    if(
      (p1_moves.find(i) != p1_moves.end()) ||
      (p2_moves.find(i) != p2_moves.end())) {
      continue;
    } else {
      available_moves.insert(i);
    }
  }

  std::vector<std::pair<int,int>> move_win_pair_vec(available_moves.size());

  int count_i_move = 0;
  auto available_moves_copy = available_moves;
  for(auto i_move: available_moves_copy){
    // for each available move, do Monte Carlo 1000 (or so) times
    available_moves.erase(i_move);

    int winning_count = 0;
      // count how many times out of mTrialTimes that the current player will win
    for(int j = 0; j < mTrialTimes; ++j){
      p1_moves_to_be_filled = p1_moves;
      p2_moves_to_be_filled = p2_moves;
      if(player_turn == EPlayerTurn::EPlayer1){
        p1_moves_to_be_filled.insert(i_move);
      } else {
        p2_moves_to_be_filled.insert(i_move);
      }
      Fill(p1_moves_to_be_filled, p2_moves_to_be_filled, available_moves);
      EWinner winner = FindWinnerOfFilledBoard(p1_moves_to_be_filled, mBoardSize);
      if((player_turn == EPlayerTurn::EPlayer1) &&
          (winner == EWinner::EPlayer1Win)){
        ++winning_count;
      } else if((player_turn == EPlayerTurn::EPlayer2) &&
          (winner == EWinner::EPlayer2Win)){
        ++winning_count;
      }
    }
    move_win_pair_vec[count_i_move] = std::pair<int,int> (i_move, winning_count);
    ++count_i_move;
    available_moves.insert(i_move);
  }
  std::sort(move_win_pair_vec.begin(), move_win_pair_vec.end(), MaxComeFirst());

  return move_win_pair_vec[0].first;
}
void MonteCarloAI::Fill(
    std::unordered_set<int> &p1_moves_to_be_filled,
    std::unordered_set<int> &p2_moves_to_be_filled,
    const std::unordered_set<int> &available_moves){

  std::vector<int> available_moves_vec (available_moves.begin(), available_moves.end());
  int seed = std::chrono::system_clock::now().time_since_epoch().count();
  int available_moves_size = available_moves.size();

  std::shuffle(available_moves_vec.begin(), available_moves_vec.end(),
    std::default_random_engine(seed)
  );

	if(available_moves_size %2 == 0){
    // if it is equally left, divide it by half
    //  whether the n*n % 2 == 1 or not
	  p1_moves_to_be_filled.insert(
	    available_moves_vec.begin(), 
	    available_moves_vec.begin() + available_moves_size / 2
	  );
	  p2_moves_to_be_filled.insert(
	    available_moves_vec.begin() + available_moves_size / 2, 
	    available_moves_vec.end()
	  );
	} else { // available_move_size is odd
		// odd available
    //   p1 get more if the board is odd
    //   p2 get more if the board is even
    if(mBoardSize % 2 == 0){
			p1_moves_to_be_filled.insert(
			  available_moves_vec.begin(), 
			  available_moves_vec.begin() + available_moves_size / 2
			);
			p2_moves_to_be_filled.insert(
			  available_moves_vec.begin() + available_moves_size / 2, 
			  available_moves_vec.end()
			);
    } else {
			p1_moves_to_be_filled.insert(
			  available_moves_vec.begin(), 
			  available_moves_vec.begin() + available_moves_size / 2 + 1
			);
			p2_moves_to_be_filled.insert(
			  available_moves_vec.begin() + available_moves_size / 2 + 1, 
			  available_moves_vec.end()
			);
    }
	}
}

} // namespace HexBE
