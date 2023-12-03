#ifndef BE_KERNEL_H_
#define BE_KERNEL_H_

#include <memory>
#include <unordered_set>
#include "be_game_state.h" //CellOwner

enum class EPlayMode;
class Game;


namespace HexBE{
  class HexBoardBE;
  class MonteCarloAI;
} // HexBE
class KernelBE{
 public:
  void Initialize(Game *game, int board_size, EPlayMode play_mode);
  void IncrementPly();
  PlayerTurn GetPlayerTurn();
  void AddP1Move(int move);
  void AddP2Move(int move);

  // called from super-class
  int FindWinner(std::unordered_set<int>& winner_move);
  int GetAiMove();

  // Called from sub-class
  EPlayMode GetPlayMode() const {return mPlayMode;}
 private:
  Game *pGame;
  int mPly = 0;
  EPlayMode mPlayMode;
  std::shared_ptr<HexBE::HexBoardBE> pHexBoardBE;
  std::shared_ptr<HexBE::MonteCarloAI> pAi;
  std::unordered_set<int> mWinnerMoves;
};

#endif // BE_HERNEL_H_
