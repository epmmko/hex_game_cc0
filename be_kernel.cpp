#include <iostream>
#include "be_kernel.h"
#include "be_game_state.h" //CellOwner
#include "be_hexboard.h"
#include "fe_game.h"
#include "be_hex_ai.h"

PlayerTurn KernelBE::GetPlayerTurn(){
  return static_cast<PlayerTurn>(mPly%2);
}

void KernelBE::Initialize(Game *game, int board_size, EPlayMode play_mode){
  mPlayMode = play_mode;
  pHexBoardBE = std::shared_ptr<HexBE::HexBoardBE> (new HexBE::HexBoardBE());
  pHexBoardBE->Initialize(board_size);
  pGame = game;
  pAi = std::shared_ptr<HexBE::MonteCarloAI> (new HexBE::MonteCarloAI());
  pAi->Initialize(board_size);
}

void KernelBE::AddP1Move(int move){
  pHexBoardBE->AddP1Move(move);
};

void KernelBE::AddP2Move(int move){
  pHexBoardBE->AddP2Move(move);
};

int KernelBE::FindWinner(std::unordered_set<int>& winner_move){
  HexBE::EWinner winner = pHexBoardBE->GetWinner(winner_move);
  switch(winner){
    case(HexBE::EWinner::EPlayer1Win):
      return 1;
      break;
    case(HexBE::EWinner::EPlayer2Win):
      return 2;
      break;
    default:
      return 0;
  }
}

int KernelBE::GetAiMove(){
  return pAi->GetAiMove(
    pHexBoardBE->GetP1Moves(),
    pHexBoardBE->GetP2Moves());
}

void KernelBE::IncrementPly(){
  ++mPly;
}
